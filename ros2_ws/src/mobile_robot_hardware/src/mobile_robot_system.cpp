#include "mobile_robot_hardware/mobile_robot_system.hpp"

#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <stdexcept>

#include "diagnostic_msgs/msg/diagnostic_status.hpp"
#include "diagnostic_msgs/msg/key_value.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "mobile_robot_hardware/protocol.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "rclcpp/rclcpp.hpp"

namespace mobile_robot_hardware {

namespace {

constexpr double kTwoPi = 6.28318530717958647692;
constexpr auto kAckTimeout = std::chrono::milliseconds(500);
constexpr auto kHeartbeatPeriod = std::chrono::milliseconds(250);

constexpr const char* kLeftWheelJoint = "left_wheel_joint";
constexpr const char* kRightWheelJoint = "right_wheel_joint";

constexpr const char* kHardwareDiagnosticName =
    "Arty-ROS2 hardware: mobile_robot_hardware";

diagnostic_msgs::msg::KeyValue make_key_value(
    const std::string& key,
    const std::string& value) {
  diagnostic_msgs::msg::KeyValue item;
  item.key = key;
  item.value = value;
  return item;
}

speed_t to_termios_baud(const int baud) {
  switch (baud) {
    case 115200:
      return B115200;
    case 57600:
      return B57600;
    case 38400:
      return B38400;
    case 19200:
      return B19200;
    case 9600:
      return B9600;
    default:
      throw std::runtime_error("Unsupported UART baud rate");
  }
}

double required_positive_parameter(
    const hardware_interface::HardwareInfo& info,
    const std::string& name) {
  const auto it = info.hardware_parameters.find(name);

  if (it == info.hardware_parameters.end()) {
    throw std::runtime_error(
        "Missing hardware parameter: " + name);
  }

  const double value = std::stod(it->second);

  if (!std::isfinite(value) || value <= 0.0) {
    throw std::runtime_error(
        "Hardware parameter must be finite and > 0: " + name);
  }

  return value;
}

int required_positive_integer_parameter(
    const hardware_interface::HardwareInfo& info,
    const std::string& name) {
  const auto it = info.hardware_parameters.find(name);

  if (it == info.hardware_parameters.end()) {
    throw std::runtime_error(
        "Missing hardware parameter: " + name);
  }

  const int value = std::stoi(it->second);

  if (value <= 0) {
    throw std::runtime_error(
        "Hardware parameter must be > 0: " + name);
  }

  return value;
}

bool required_boolean_parameter(
    const hardware_interface::HardwareInfo& info,
    const std::string& name) {
  const auto it = info.hardware_parameters.find(name);

  if (it == info.hardware_parameters.end()) {
    throw std::runtime_error(
        "Missing hardware parameter: " + name);
  }

  if (it->second == "true" || it->second == "1") {
    return true;
  }

  if (it->second == "false" || it->second == "0") {
    return false;
  }

  throw std::runtime_error(
      "Hardware parameter must be true/false or 1/0: " + name);
}

bool validate_joint_interfaces(
    const hardware_interface::ComponentInfo& joint) {
  if (
      joint.command_interfaces.size() != 1U ||
      joint.command_interfaces[0].name !=
          hardware_interface::HW_IF_VELOCITY) {
    return false;
  }

  if (joint.state_interfaces.size() != 2U) {
    return false;
  }

  bool has_position = false;
  bool has_velocity = false;

  for (const auto& state : joint.state_interfaces) {
    if (state.name == hardware_interface::HW_IF_POSITION) {
      has_position = true;
    } else if (
        state.name == hardware_interface::HW_IF_VELOCITY) {
      has_velocity = true;
    } else {
      return false;
    }
  }

  return has_position && has_velocity;
}

}  // namespace

hardware_interface::CallbackReturn
MobileRobotSystem::on_init(
    const hardware_interface::HardwareInfo& info) {
  if (
      hardware_interface::SystemInterface::on_init(info) !=
      hardware_interface::CallbackReturn::SUCCESS) {
    return hardware_interface::CallbackReturn::ERROR;
  }

  if (info_.joints.size() != 2U) {
    RCLCPP_ERROR(
        get_logger(),
        "Exactly two wheel joints are required; received %zu",
        info_.joints.size());

    return hardware_interface::CallbackReturn::ERROR;
  }

  bool have_left = false;
  bool have_right = false;

  for (std::size_t index = 0U;
       index < info_.joints.size();
       ++index) {
    const auto& joint = info_.joints[index];

    if (!validate_joint_interfaces(joint)) {
      RCLCPP_ERROR(
          get_logger(),
          "Joint '%s' must contain exactly one velocity command interface and exactly position+velocity state interfaces",
          joint.name.c_str());

      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.name == kLeftWheelJoint) {
      if (have_left) {
        RCLCPP_ERROR(
            get_logger(),
            "Duplicate joint '%s'",
            kLeftWheelJoint);

        return hardware_interface::CallbackReturn::ERROR;
      }

      left_joint_index_ = index;
      have_left = true;
      continue;
    }

    if (joint.name == kRightWheelJoint) {
      if (have_right) {
        RCLCPP_ERROR(
            get_logger(),
            "Duplicate joint '%s'",
            kRightWheelJoint);

        return hardware_interface::CallbackReturn::ERROR;
      }

      right_joint_index_ = index;
      have_right = true;
      continue;
    }

    RCLCPP_ERROR(
        get_logger(),
        "Unsupported joint '%s'; expected '%s' and '%s'",
        joint.name.c_str(),
        kLeftWheelJoint,
        kRightWheelJoint);

    return hardware_interface::CallbackReturn::ERROR;
  }

  if (!have_left || !have_right) {
    RCLCPP_ERROR(
        get_logger(),
        "Both '%s' and '%s' must be present",
        kLeftWheelJoint,
        kRightWheelJoint);

    return hardware_interface::CallbackReturn::ERROR;
  }

  try {
    const auto device_it =
        info_.hardware_parameters.find("serial_device");

    const auto baud_it =
        info_.hardware_parameters.find("baud_rate");

    if (
        device_it == info_.hardware_parameters.end() ||
        baud_it == info_.hardware_parameters.end()) {
      throw std::runtime_error(
          "serial_device and baud_rate are required");
    }

    serial_device_ = device_it->second;
    baud_rate_ = std::stoi(baud_it->second);

    if (serial_device_.empty()) {
      throw std::runtime_error(
          "serial_device must not be empty");
    }

    (void)to_termios_baud(baud_rate_);

    ticks_per_revolution_ =
        required_positive_parameter(
            info_,
            "ticks_per_revolution");

    telemetry_timeout_ms_ =
        required_positive_integer_parameter(
            info_,
            "telemetry_timeout_ms");

    left_motor_inverted_ =
        required_boolean_parameter(
            info_,
            "left_motor_inverted");

    right_motor_inverted_ =
        required_boolean_parameter(
            info_,
            "right_motor_inverted");

    left_encoder_inverted_ =
        required_boolean_parameter(
            info_,
            "left_encoder_inverted");

    right_encoder_inverted_ =
        required_boolean_parameter(
            info_,
            "right_encoder_inverted");
  } catch (const std::exception& error) {
    RCLCPP_ERROR(
        get_logger(),
        "%s",
        error.what());

    return hardware_interface::CallbackReturn::ERROR;
  }

  setup_diagnostics();

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn
MobileRobotSystem::on_configure(
    const rclcpp_lifecycle::State&) {
  if (!open_serial()) {
    return hardware_interface::CallbackReturn::ERROR;
  }

  if (!configure_firmware()) {
    RCLCPP_ERROR(
        get_logger(),
        "ESP32 rejected configuration handshake");

    close_serial();
    return hardware_interface::CallbackReturn::ERROR;
  }

  if (!clear_firmware_faults()) {
    RCLCPP_ERROR(
        get_logger(),
        "ESP32 rejected clear-fault transaction");

    close_serial();
    return hardware_interface::CallbackReturn::ERROR;
  }

  left_position_ = 0.0;
  right_position_ = 0.0;
  left_velocity_ = 0.0;
  right_velocity_ = 0.0;
  left_command_ = 0.0;
  right_command_ = 0.0;

  last_fault_flags_ = 0U;
  have_received_telemetry_ = false;

  diagnostic_configured_.store(
      true,
      std::memory_order_relaxed);
  diagnostic_enabled_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_have_telemetry_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_fault_flags_.store(
      0U,
      std::memory_order_relaxed);
  diagnostic_last_telemetry_ns_.store(
      0,
      std::memory_order_relaxed);

  const auto now = std::chrono::steady_clock::now();
  last_heartbeat_ = now;
  last_telemetry_ = now;

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn
MobileRobotSystem::on_activate(
    const rclcpp_lifecycle::State&) {
  left_command_ = 0.0;
  right_command_ = 0.0;

  if (!set_firmware_enabled(true)) {
    RCLCPP_ERROR(
        get_logger(),
        "ESP32 rejected enable command");

    return hardware_interface::CallbackReturn::ERROR;
  }

  const auto now = std::chrono::steady_clock::now();
  last_heartbeat_ = now;
  last_telemetry_ = now;
  have_received_telemetry_ = false;

  diagnostic_enabled_.store(
      true,
      std::memory_order_relaxed);
  diagnostic_have_telemetry_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_last_telemetry_ns_.store(
      0,
      std::memory_order_relaxed);

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn
MobileRobotSystem::on_deactivate(
    const rclcpp_lifecycle::State&) {
  left_command_ = 0.0;
  right_command_ = 0.0;

  (void)send_velocity();

  if (!set_firmware_enabled(false)) {
    RCLCPP_ERROR(
        get_logger(),
        "Failed to confirm ESP32 disable command");

    return hardware_interface::CallbackReturn::ERROR;
  }

  diagnostic_enabled_.store(
      false,
      std::memory_order_relaxed);

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn
MobileRobotSystem::on_cleanup(
    const rclcpp_lifecycle::State&) {
  if (serial_fd_ >= 0) {
    (void)set_firmware_enabled(false);
  }

  close_serial();

  have_received_telemetry_ = false;
  last_fault_flags_ = 0U;

  diagnostic_configured_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_enabled_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_have_telemetry_.store(
      false,
      std::memory_order_relaxed);
  diagnostic_fault_flags_.store(
      0U,
      std::memory_order_relaxed);
  diagnostic_last_telemetry_ns_.store(
      0,
      std::memory_order_relaxed);

  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
MobileRobotSystem::export_state_interfaces() {
  std::vector<hardware_interface::StateInterface> interfaces;

  interfaces.emplace_back(
      info_.joints[left_joint_index_].name,
      hardware_interface::HW_IF_POSITION,
      &left_position_);

  interfaces.emplace_back(
      info_.joints[left_joint_index_].name,
      hardware_interface::HW_IF_VELOCITY,
      &left_velocity_);

  interfaces.emplace_back(
      info_.joints[right_joint_index_].name,
      hardware_interface::HW_IF_POSITION,
      &right_position_);

  interfaces.emplace_back(
      info_.joints[right_joint_index_].name,
      hardware_interface::HW_IF_VELOCITY,
      &right_velocity_);

  return interfaces;
}

std::vector<hardware_interface::CommandInterface>
MobileRobotSystem::export_command_interfaces() {
  std::vector<hardware_interface::CommandInterface> interfaces;

  interfaces.emplace_back(
      info_.joints[left_joint_index_].name,
      hardware_interface::HW_IF_VELOCITY,
      &left_command_);

  interfaces.emplace_back(
      info_.joints[right_joint_index_].name,
      hardware_interface::HW_IF_VELOCITY,
      &right_command_);

  return interfaces;
}

hardware_interface::return_type
MobileRobotSystem::read(
    const rclcpp::Time&,
    const rclcpp::Duration&) {
  std::vector<uint8_t> frame;

  while (
      read_one_frame(
          frame,
          std::chrono::milliseconds(1))) {
    const auto packet =
        protocol::decode_frame(frame);

    if (
        !packet.has_value() ||
        packet->header.type !=
            static_cast<uint8_t>(
                protocol::MessageType::kTelemetry)) {
      continue;
    }

    protocol::TelemetryPayload telemetry{};

    if (!protocol::decode_payload(*packet, telemetry)) {
      RCLCPP_ERROR(
          get_logger(),
          "Invalid telemetry payload size");

      return hardware_interface::return_type::ERROR;
    }

    if (telemetry.configured == 0U) {
      RCLCPP_ERROR(
          get_logger(),
          "ESP32 lost configured state");

      return hardware_interface::return_type::ERROR;
    }

    if (
        !std::isfinite(
            telemetry.left_velocity_rad_per_sec) ||
        !std::isfinite(
            telemetry.right_velocity_rad_per_sec)) {
      RCLCPP_ERROR(
          get_logger(),
          "ESP32 returned non-finite wheel velocity");

      return hardware_interface::return_type::ERROR;
    }

    left_position_ =
        static_cast<double>(telemetry.left_ticks) *
        kTwoPi /
        ticks_per_revolution_;

    right_position_ =
        static_cast<double>(telemetry.right_ticks) *
        kTwoPi /
        ticks_per_revolution_;

    left_velocity_ =
        telemetry.left_velocity_rad_per_sec;

    right_velocity_ =
        telemetry.right_velocity_rad_per_sec;

    last_fault_flags_ =
        telemetry.fault_flags;

    last_telemetry_ =
        std::chrono::steady_clock::now();

    have_received_telemetry_ = true;

    diagnostic_configured_.store(
        telemetry.configured != 0U,
        std::memory_order_relaxed);
    diagnostic_enabled_.store(
        telemetry.enabled != 0U,
        std::memory_order_relaxed);
    diagnostic_have_telemetry_.store(
        true,
        std::memory_order_relaxed);
    diagnostic_fault_flags_.store(
        telemetry.fault_flags,
        std::memory_order_relaxed);
    diagnostic_last_telemetry_ns_.store(
        steady_now_nanoseconds(),
        std::memory_order_relaxed);
    diagnostic_left_ticks_.store(
        telemetry.left_ticks,
        std::memory_order_relaxed);
    diagnostic_right_ticks_.store(
        telemetry.right_ticks,
        std::memory_order_relaxed);
    diagnostic_left_velocity_.store(
        static_cast<double>(
            telemetry.left_velocity_rad_per_sec),
        std::memory_order_relaxed);
    diagnostic_right_velocity_.store(
        static_cast<double>(
            telemetry.right_velocity_rad_per_sec),
        std::memory_order_relaxed);
  }

  const auto now =
      std::chrono::steady_clock::now();

  if (
      now - last_telemetry_ >
      std::chrono::milliseconds(
          telemetry_timeout_ms_)) {
    RCLCPP_ERROR(
        get_logger(),
        "ESP32 telemetry timeout: no valid telemetry for more than %d ms",
        telemetry_timeout_ms_);

    return hardware_interface::return_type::ERROR;
  }

  if (last_fault_flags_ != 0U) {
    RCLCPP_ERROR(
        get_logger(),
        "ESP32 fault_flags=0x%08X",
        static_cast<unsigned int>(
            last_fault_flags_));

    return hardware_interface::return_type::ERROR;
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type
MobileRobotSystem::write(
    const rclcpp::Time&,
    const rclcpp::Duration&) {
  if (
      !std::isfinite(left_command_) ||
      !std::isfinite(right_command_)) {
    RCLCPP_ERROR(
        get_logger(),
        "Non-finite wheel command rejected");

    return hardware_interface::return_type::ERROR;
  }

  if (!send_velocity()) {
    return hardware_interface::return_type::ERROR;
  }

  const auto now =
      std::chrono::steady_clock::now();

  if (
      now - last_heartbeat_ >=
      kHeartbeatPeriod) {
    if (!send_heartbeat()) {
      return hardware_interface::return_type::ERROR;
    }

    last_heartbeat_ = now;
  }

  return hardware_interface::return_type::OK;
}

bool MobileRobotSystem::open_serial() {
  close_serial();

  serial_parser_.reset();
  pending_frames_.clear();

  serial_fd_ =
      ::open(
          serial_device_.c_str(),
          O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (serial_fd_ < 0) {
    RCLCPP_ERROR(
        get_logger(),
        "Cannot open serial device '%s': %s",
        serial_device_.c_str(),
        std::strerror(errno));

    return false;
  }

  termios tty{};

  if (tcgetattr(serial_fd_, &tty) != 0) {
    RCLCPP_ERROR(
        get_logger(),
        "tcgetattr failed: %s",
        std::strerror(errno));

    close_serial();
    return false;
  }

  const speed_t speed =
      to_termios_baud(baud_rate_);

  cfsetispeed(&tty, speed);
  cfsetospeed(&tty, speed);

  tty.c_cflag =
      (tty.c_cflag & ~CSIZE) | CS8;

  tty.c_cflag |= CLOCAL | CREAD;
  tty.c_cflag &=
      ~(PARENB | CSTOPB | CRTSCTS);

  tty.c_iflag = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 0;

  tcflush(serial_fd_, TCIOFLUSH);

  if (tcsetattr(
          serial_fd_,
          TCSANOW,
          &tty) != 0) {
    RCLCPP_ERROR(
        get_logger(),
        "tcsetattr failed: %s",
        std::strerror(errno));

    close_serial();
    return false;
  }

  return true;
}

void MobileRobotSystem::close_serial() {
  if (serial_fd_ >= 0) {
    ::close(serial_fd_);
    serial_fd_ = -1;
  }

  serial_parser_.reset();
  pending_frames_.clear();
}

bool MobileRobotSystem::configure_firmware() {
  const uint16_t sequence =
      sequence_++;

  const protocol::ConfigurePayload payload{
      static_cast<float>(ticks_per_revolution_),
      static_cast<uint8_t>(
          left_motor_inverted_ ? 1U : 0U),
      static_cast<uint8_t>(
          right_motor_inverted_ ? 1U : 0U),
      static_cast<uint8_t>(
          left_encoder_inverted_ ? 1U : 0U),
      static_cast<uint8_t>(
          right_encoder_inverted_ ? 1U : 0U),
  };

  return write_all(
             protocol::encode_frame(
                 protocol::MessageType::kCommandConfigure,
                 sequence,
                 payload)) &&
         wait_for_ack(
             sequence,
             static_cast<uint8_t>(
                 protocol::MessageType::kCommandConfigure),
             kAckTimeout);
}

bool MobileRobotSystem::clear_firmware_faults() {
  const uint16_t sequence =
      sequence_++;

  const protocol::ClearFaultsPayload payload{0U};

  return write_all(
             protocol::encode_frame(
                 protocol::MessageType::kCommandClearFaults,
                 sequence,
                 payload)) &&
         wait_for_ack(
             sequence,
             static_cast<uint8_t>(
                 protocol::MessageType::kCommandClearFaults),
             kAckTimeout);
}

bool MobileRobotSystem::set_firmware_enabled(
    const bool enabled) {
  const uint16_t sequence =
      sequence_++;

  const protocol::EnablePayload payload{
      static_cast<uint8_t>(
          enabled ? 1U : 0U),
  };

  return write_all(
             protocol::encode_frame(
                 protocol::MessageType::kCommandEnable,
                 sequence,
                 payload)) &&
         wait_for_ack(
             sequence,
             static_cast<uint8_t>(
                 protocol::MessageType::kCommandEnable),
             kAckTimeout);
}

bool MobileRobotSystem::send_velocity() {
  const uint16_t sequence =
      sequence_++;

  const protocol::VelocityPayload payload{
      static_cast<float>(left_command_),
      static_cast<float>(right_command_),
  };

  return write_all(
      protocol::encode_frame(
          protocol::MessageType::kCommandVelocity,
          sequence,
          payload));
}

bool MobileRobotSystem::send_heartbeat() {
  const uint16_t sequence =
      sequence_++;

  const protocol::HeartbeatPayload payload{0U};

  return write_all(
      protocol::encode_frame(
          protocol::MessageType::kHeartbeat,
          sequence,
          payload));
}

bool MobileRobotSystem::wait_for_ack(
    const uint16_t sequence,
    const uint8_t command_type,
    const std::chrono::milliseconds timeout) {
  const auto deadline =
      std::chrono::steady_clock::now() +
      timeout;

  std::vector<uint8_t> frame;

  while (
      std::chrono::steady_clock::now() <
      deadline) {
    const auto remaining =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
            deadline -
            std::chrono::steady_clock::now());

    if (!read_one_frame(frame, remaining)) {
      continue;
    }

    const auto packet =
        protocol::decode_frame(frame);

    if (
        !packet.has_value() ||
        packet->header.type !=
            static_cast<uint8_t>(
                protocol::MessageType::kAck) ||
        packet->header.sequence !=
            sequence) {
      continue;
    }

    protocol::AckPayload ack{};

    if (!protocol::decode_payload(*packet, ack)) {
      return false;
    }

    return
        ack.command_type == command_type &&
        ack.status ==
            static_cast<uint8_t>(
                protocol::AckStatus::kAccepted);
  }

  return false;
}

bool MobileRobotSystem::read_one_frame(
    std::vector<uint8_t>& frame,
    const std::chrono::milliseconds timeout) {
  frame.clear();

  if (!pending_frames_.empty()) {
    frame = std::move(
        pending_frames_.front());

    pending_frames_.pop_front();
    return true;
  }

  if (serial_fd_ < 0) {
    return false;
  }

  const auto deadline =
      std::chrono::steady_clock::now() +
      timeout;

  std::array<uint8_t, 128U> buffer{};

  while (
      std::chrono::steady_clock::now() <
      deadline) {
    const auto now =
        std::chrono::steady_clock::now();

    const auto remaining =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
            deadline - now);

    pollfd descriptor{
        serial_fd_,
        POLLIN,
        0,
    };

    const int poll_result =
        ::poll(
            &descriptor,
            1,
            static_cast<int>(
                std::max<int64_t>(
                    1,
                    remaining.count())));

    if (poll_result < 0) {
      if (errno == EINTR) {
        continue;
      }

      return false;
    }

    if (poll_result == 0) {
      return false;
    }

    if (
        (descriptor.revents &
         (POLLERR | POLLHUP | POLLNVAL)) !=
        0) {
      return false;
    }

    if (
        (descriptor.revents & POLLIN) ==
        0) {
      continue;
    }

    const ssize_t count =
        ::read(
            serial_fd_,
            buffer.data(),
            buffer.size());

    if (count < 0) {
      if (
          errno == EAGAIN ||
          errno == EWOULDBLOCK ||
          errno == EINTR) {
        continue;
      }

      return false;
    }

    if (count == 0) {
      continue;
    }

    for (ssize_t index = 0;
         index < count;
         ++index) {
      auto completed =
          serial_parser_.push(
              buffer[
                  static_cast<std::size_t>(
                      index)]);

      if (!completed.has_value()) {
        continue;
      }

      if (frame.empty()) {
        frame = std::move(
            *completed);
      } else {
        pending_frames_.push_back(
            std::move(*completed));
      }
    }

    if (!frame.empty()) {
      return true;
    }
  }

  return false;
}

bool MobileRobotSystem::write_all(
    const std::vector<uint8_t>& data) {
  if (serial_fd_ < 0) {
    return false;
  }

  std::size_t written = 0U;

  while (written < data.size()) {
    const ssize_t count =
        ::write(
            serial_fd_,
            data.data() + written,
            data.size() - written);

    if (count < 0) {
      if (
          errno == EAGAIN ||
          errno == EWOULDBLOCK) {
        pollfd descriptor{
            serial_fd_,
            POLLOUT,
            0,
        };

        if (::poll(&descriptor, 1, 100) <= 0) {
          return false;
        }

        continue;
      }

      return false;
    }

    written +=
        static_cast<std::size_t>(
            count);
  }

  return true;
}

int64_t MobileRobotSystem::steady_now_nanoseconds() {
  return std::chrono::duration_cast<
      std::chrono::nanoseconds>(
      std::chrono::steady_clock::now()
          .time_since_epoch())
      .count();
}

void MobileRobotSystem::setup_diagnostics() {
  const auto node = get_node();

  if (!node) {
    RCLCPP_WARN(
        get_logger(),
        "ros2_control framework node unavailable; hardware diagnostics disabled");
    return;
  }

  diagnostics_publisher_ =
      node->create_publisher<
          diagnostic_msgs::msg::DiagnosticArray>(
          "/diagnostics",
          rclcpp::QoS(10));

  diagnostics_timer_ =
      node->create_wall_timer(
          std::chrono::milliseconds(500),
          [this]() {
            publish_hardware_diagnostics();
          });
}

void MobileRobotSystem::publish_hardware_diagnostics() {
  if (!diagnostics_publisher_) {
    return;
  }

  const auto node = get_node();

  if (!node) {
    return;
  }

  diagnostic_msgs::msg::DiagnosticArray array;
  array.header.stamp =
      node->get_clock()->now();

  diagnostic_msgs::msg::DiagnosticStatus status;
  status.name = kHardwareDiagnosticName;
  status.hardware_id = "arty_ros2_base";

  const bool configured =
      diagnostic_configured_.load(
          std::memory_order_relaxed);

  const bool enabled =
      diagnostic_enabled_.load(
          std::memory_order_relaxed);

  const bool have_telemetry =
      diagnostic_have_telemetry_.load(
          std::memory_order_relaxed);

  const uint32_t fault_flags =
      diagnostic_fault_flags_.load(
          std::memory_order_relaxed);

  const int64_t last_telemetry_ns =
      diagnostic_last_telemetry_ns_.load(
          std::memory_order_relaxed);

  double telemetry_age_ms = -1.0;

  if (
      have_telemetry &&
      last_telemetry_ns > 0) {
    telemetry_age_ms =
        static_cast<double>(
            steady_now_nanoseconds() -
            last_telemetry_ns) /
        1'000'000.0;
  }

  if (!configured) {
    status.level =
        diagnostic_msgs::msg::
            DiagnosticStatus::WARN;
    status.message =
        "hardware not configured";
  } else if (!have_telemetry) {
    status.level =
        diagnostic_msgs::msg::
            DiagnosticStatus::WARN;
    status.message =
        "waiting for ESP32 telemetry";
  } else if (
      telemetry_age_ms >
      static_cast<double>(
          telemetry_timeout_ms_)) {
    status.level =
        diagnostic_msgs::msg::
            DiagnosticStatus::ERROR;
    status.message =
        "ESP32 telemetry timeout";
  } else if (fault_flags != 0U) {
    status.level =
        diagnostic_msgs::msg::
            DiagnosticStatus::ERROR;
    status.message =
        "ESP32 fault detected";
  } else {
    status.level =
        diagnostic_msgs::msg::
            DiagnosticStatus::OK;
    status.message =
        enabled
            ? "hardware active"
            : "hardware configured";
  }

  status.values = {
      make_key_value(
          "serial_device",
          serial_device_),
      make_key_value(
          "baud_rate",
          std::to_string(
              baud_rate_)),
      make_key_value(
          "ticks_per_revolution",
          std::to_string(
              ticks_per_revolution_)),
      make_key_value(
          "telemetry_timeout_ms",
          std::to_string(
              telemetry_timeout_ms_)),
      make_key_value(
          "configured",
          configured
              ? "true"
              : "false"),
      make_key_value(
          "enabled",
          enabled
              ? "true"
              : "false"),
      make_key_value(
          "have_telemetry",
          have_telemetry
              ? "true"
              : "false"),
      make_key_value(
          "telemetry_age_ms",
          telemetry_age_ms < 0.0
              ? "never"
              : std::to_string(
                    telemetry_age_ms)),
      make_key_value(
          "fault_flags",
          std::to_string(
              fault_flags)),
      make_key_value(
          "left_ticks",
          std::to_string(
              diagnostic_left_ticks_.load(
                  std::memory_order_relaxed))),
      make_key_value(
          "right_ticks",
          std::to_string(
              diagnostic_right_ticks_.load(
                  std::memory_order_relaxed))),
      make_key_value(
          "left_velocity_rad_per_sec",
          std::to_string(
              diagnostic_left_velocity_.load(
                  std::memory_order_relaxed))),
      make_key_value(
          "right_velocity_rad_per_sec",
          std::to_string(
              diagnostic_right_velocity_.load(
                  std::memory_order_relaxed))),
      make_key_value(
          "left_motor_inverted",
          left_motor_inverted_
              ? "true"
              : "false"),
      make_key_value(
          "right_motor_inverted",
          right_motor_inverted_
              ? "true"
              : "false"),
      make_key_value(
          "left_encoder_inverted",
          left_encoder_inverted_
              ? "true"
              : "false"),
      make_key_value(
          "right_encoder_inverted",
          right_encoder_inverted_
              ? "true"
              : "false"),
  };

  array.status.push_back(
      std::move(status));

  diagnostics_publisher_->publish(
      array);
}

}  // namespace mobile_robot_hardware

PLUGINLIB_EXPORT_CLASS(
    mobile_robot_hardware::MobileRobotSystem,
    hardware_interface::SystemInterface)
