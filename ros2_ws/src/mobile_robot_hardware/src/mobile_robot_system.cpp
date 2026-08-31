#include "mobile_robot_hardware/mobile_robot_system.hpp"

#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

#include <cerrno>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <utility>

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
constexpr auto kDiagnosticsPeriod = std::chrono::milliseconds(500);

speed_t to_termios_baud(const int baud) {
  switch (baud) {
    case 115200: return B115200;
    case 57600: return B57600;
    case 38400: return B38400;
    case 19200: return B19200;
    case 9600: return B9600;
    default: throw std::runtime_error("Unsupported UART baud rate");
  }
}

double required_positive_parameter(const hardware_interface::HardwareInfo& info, const std::string& name) {
  const auto it = info.hardware_parameters.find(name);
  if (it == info.hardware_parameters.end()) throw std::runtime_error("Missing hardware parameter: " + name);
  const double value = std::stod(it->second);
  if (!std::isfinite(value) || value <= 0.0) throw std::runtime_error("Hardware parameter must be finite and > 0: " + name);
  return value;
}

int optional_positive_integer_parameter(const hardware_interface::HardwareInfo& info, const std::string& name, const int default_value) {
  const auto it = info.hardware_parameters.find(name);
  if (it == info.hardware_parameters.end()) return default_value;
  const int value = std::stoi(it->second);
  if (value <= 0) throw std::runtime_error("Hardware parameter must be > 0: " + name);
  return value;
}

int64_t steady_now_nanoseconds() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

diagnostic_msgs::msg::KeyValue key_value(const std::string& key, const std::string& value) {
  diagnostic_msgs::msg::KeyValue output;
  output.key = key;
  output.value = value;
  return output;
}
}  // namespace

hardware_interface::CallbackReturn MobileRobotSystem::on_init(const hardware_interface::HardwareInfo& info) {
  if (hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS) return hardware_interface::CallbackReturn::ERROR;
  if (info_.joints.size() != 2U) {
    RCLCPP_ERROR(get_logger(), "Exactly two wheel joints are required; received %zu", info_.joints.size());
    return hardware_interface::CallbackReturn::ERROR;
  }
  for (const auto& joint : info_.joints) {
    if (joint.command_interfaces.size() != 1U || joint.command_interfaces[0].name != hardware_interface::HW_IF_VELOCITY) {
      RCLCPP_ERROR(get_logger(), "Joint '%s' must export exactly one velocity command interface", joint.name.c_str());
      return hardware_interface::CallbackReturn::ERROR;
    }
    bool has_position = false;
    bool has_velocity = false;
    for (const auto& state : joint.state_interfaces) {
      has_position = has_position || state.name == hardware_interface::HW_IF_POSITION;
      has_velocity = has_velocity || state.name == hardware_interface::HW_IF_VELOCITY;
    }
    if (!has_position || !has_velocity) {
      RCLCPP_ERROR(get_logger(), "Joint '%s' must export position and velocity state interfaces", joint.name.c_str());
      return hardware_interface::CallbackReturn::ERROR;
    }
  }
  try {
    const auto device_it = info_.hardware_parameters.find("serial_device");
    const auto baud_it = info_.hardware_parameters.find("baud_rate");
    if (device_it == info_.hardware_parameters.end() || baud_it == info_.hardware_parameters.end()) throw std::runtime_error("serial_device and baud_rate are required");
    serial_device_ = device_it->second;
    baud_rate_ = std::stoi(baud_it->second);
    ticks_per_revolution_ = required_positive_parameter(info_, "ticks_per_revolution");
    telemetry_timeout_ms_ = optional_positive_integer_parameter(info_, "telemetry_timeout_ms", 500);
  } catch (const std::exception& error) {
    RCLCPP_ERROR(get_logger(), "%s", error.what());
    return hardware_interface::CallbackReturn::ERROR;
  }
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MobileRobotSystem::on_configure(const rclcpp_lifecycle::State&) {
  if (!open_serial()) return hardware_interface::CallbackReturn::ERROR;
  if (!configure_firmware()) {
    RCLCPP_ERROR(get_logger(), "ESP32 rejected configuration handshake");
    close_serial();
    return hardware_interface::CallbackReturn::ERROR;
  }
  left_position_ = right_position_ = left_velocity_ = right_velocity_ = left_command_ = right_command_ = 0.0;
  diagnostic_last_telemetry_ns_.store(0);
  diagnostic_left_ticks_.store(0);
  diagnostic_right_ticks_.store(0);
  diagnostic_left_velocity_.store(0.0);
  diagnostic_right_velocity_.store(0.0);
  diagnostic_fault_flags_.store(0U);
  diagnostic_configured_.store(true);
  diagnostic_enabled_.store(false);
  last_heartbeat_ = std::chrono::steady_clock::now();
  start_diagnostics();
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MobileRobotSystem::on_activate(const rclcpp_lifecycle::State&) {
  left_command_ = right_command_ = 0.0;
  if (!set_firmware_enabled(true)) {
    diagnostic_enabled_.store(false);
    RCLCPP_ERROR(get_logger(), "ESP32 rejected enable command");
    return hardware_interface::CallbackReturn::ERROR;
  }
  diagnostic_enabled_.store(true);
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MobileRobotSystem::on_deactivate(const rclcpp_lifecycle::State&) {
  left_command_ = right_command_ = 0.0;
  send_velocity();
  if (!set_firmware_enabled(false)) {
    RCLCPP_ERROR(get_logger(), "Failed to confirm ESP32 disable command");
    return hardware_interface::CallbackReturn::ERROR;
  }
  diagnostic_enabled_.store(false);
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MobileRobotSystem::on_cleanup(const rclcpp_lifecycle::State&) {
  if (serial_fd_ >= 0) set_firmware_enabled(false);
  diagnostic_enabled_.store(false);
  diagnostic_configured_.store(false);
  stop_diagnostics();
  close_serial();
  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> MobileRobotSystem::export_state_interfaces() {
  std::vector<hardware_interface::StateInterface> v;
  v.emplace_back(info_.joints[0].name, hardware_interface::HW_IF_POSITION, &left_position_);
  v.emplace_back(info_.joints[0].name, hardware_interface::HW_IF_VELOCITY, &left_velocity_);
  v.emplace_back(info_.joints[1].name, hardware_interface::HW_IF_POSITION, &right_position_);
  v.emplace_back(info_.joints[1].name, hardware_interface::HW_IF_VELOCITY, &right_velocity_);
  return v;
}

std::vector<hardware_interface::CommandInterface> MobileRobotSystem::export_command_interfaces() {
  std::vector<hardware_interface::CommandInterface> v;
  v.emplace_back(info_.joints[0].name, hardware_interface::HW_IF_VELOCITY, &left_command_);
  v.emplace_back(info_.joints[1].name, hardware_interface::HW_IF_VELOCITY, &right_command_);
  return v;
}

hardware_interface::return_type MobileRobotSystem::read(const rclcpp::Time&, const rclcpp::Duration&) {
  std::vector<uint8_t> frame;
  while (read_one_frame(frame, std::chrono::milliseconds(1))) {
    const auto packet = protocol::decode_frame(frame);
    if (!packet || packet->header.type != static_cast<uint8_t>(protocol::MessageType::kTelemetry)) continue;
    protocol::TelemetryPayload telemetry{};
    if (!protocol::decode_payload(*packet, telemetry)) continue;
    diagnostic_last_telemetry_ns_.store(steady_now_nanoseconds(), std::memory_order_relaxed);
    diagnostic_left_ticks_.store(telemetry.left_ticks, std::memory_order_relaxed);
    diagnostic_right_ticks_.store(telemetry.right_ticks, std::memory_order_relaxed);
    diagnostic_left_velocity_.store(telemetry.left_velocity_rad_per_sec, std::memory_order_relaxed);
    diagnostic_right_velocity_.store(telemetry.right_velocity_rad_per_sec, std::memory_order_relaxed);
    diagnostic_fault_flags_.store(telemetry.fault_flags, std::memory_order_relaxed);
    diagnostic_configured_.store(telemetry.configured != 0U, std::memory_order_relaxed);
    diagnostic_enabled_.store(telemetry.enabled != 0U, std::memory_order_relaxed);
    if (telemetry.configured == 0U) {
      RCLCPP_ERROR(get_logger(), "ESP32 lost configured state");
      return hardware_interface::return_type::ERROR;
    }
    left_position_ = static_cast<double>(telemetry.left_ticks) * kTwoPi / ticks_per_revolution_;
    right_position_ = static_cast<double>(telemetry.right_ticks) * kTwoPi / ticks_per_revolution_;
    left_velocity_ = telemetry.left_velocity_rad_per_sec;
    right_velocity_ = telemetry.right_velocity_rad_per_sec;
  }
  return hardware_interface::return_type::OK;
}

hardware_interface::return_type MobileRobotSystem::write(const rclcpp::Time&, const rclcpp::Duration&) {
  if (!std::isfinite(left_command_) || !std::isfinite(right_command_)) {
    RCLCPP_ERROR(get_logger(), "Non-finite wheel command rejected");
    return hardware_interface::return_type::ERROR;
  }
  if (!send_velocity()) return hardware_interface::return_type::ERROR;
  const auto now = std::chrono::steady_clock::now();
  if (now - last_heartbeat_ >= kHeartbeatPeriod) {
    if (!send_heartbeat()) return hardware_interface::return_type::ERROR;
    last_heartbeat_ = now;
  }
  return hardware_interface::return_type::OK;
}

bool MobileRobotSystem::open_serial() {
  close_serial();
  serial_fd_ = ::open(serial_device_.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (serial_fd_ < 0) {
    RCLCPP_ERROR(get_logger(), "Cannot open serial device '%s': %s", serial_device_.c_str(), std::strerror(errno));
    return false;
  }
  termios tty{};
  if (tcgetattr(serial_fd_, &tty) != 0) { close_serial(); return false; }
  const speed_t speed = to_termios_baud(baud_rate_);
  cfsetispeed(&tty, speed); cfsetospeed(&tty, speed);
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
  tty.c_cflag |= CLOCAL | CREAD;
  tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
  tty.c_iflag = 0; tty.c_oflag = 0; tty.c_lflag = 0;
  tty.c_cc[VMIN] = 0; tty.c_cc[VTIME] = 0;
  tcflush(serial_fd_, TCIOFLUSH);
  if (tcsetattr(serial_fd_, TCSANOW, &tty) != 0) { close_serial(); return false; }
  return true;
}

void MobileRobotSystem::close_serial() { if (serial_fd_ >= 0) { ::close(serial_fd_); serial_fd_ = -1; } }

bool MobileRobotSystem::configure_firmware() {
  const uint16_t seq = sequence_++;
  const protocol::ConfigurePayload p{static_cast<float>(ticks_per_revolution_)};
  return write_all(protocol::encode_frame(protocol::MessageType::kCommandConfigure, seq, p)) && wait_for_ack(seq, static_cast<uint8_t>(protocol::MessageType::kCommandConfigure), kAckTimeout);
}

bool MobileRobotSystem::set_firmware_enabled(const bool enabled) {
  const uint16_t seq = sequence_++;
  const protocol::EnablePayload p{static_cast<uint8_t>(enabled ? 1U : 0U)};
  return write_all(protocol::encode_frame(protocol::MessageType::kCommandEnable, seq, p)) && wait_for_ack(seq, static_cast<uint8_t>(protocol::MessageType::kCommandEnable), kAckTimeout);
}

bool MobileRobotSystem::send_velocity() {
  const uint16_t seq = sequence_++;
  const protocol::VelocityPayload p{static_cast<float>(left_command_), static_cast<float>(right_command_)};
  return write_all(protocol::encode_frame(protocol::MessageType::kCommandVelocity, seq, p));
}

bool MobileRobotSystem::send_heartbeat() {
  const uint16_t seq = sequence_++;
  const protocol::HeartbeatPayload p{0U};
  return write_all(protocol::encode_frame(protocol::MessageType::kHeartbeat, seq, p));
}

bool MobileRobotSystem::wait_for_ack(uint16_t sequence, uint8_t command_type, std::chrono::milliseconds timeout) {
  const auto deadline = std::chrono::steady_clock::now() + timeout;
  std::vector<uint8_t> frame;
  while (std::chrono::steady_clock::now() < deadline) {
    const auto rem = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
    if (!read_one_frame(frame, rem)) continue;
    const auto packet = protocol::decode_frame(frame);
    if (!packet || packet->header.type != static_cast<uint8_t>(protocol::MessageType::kAck) || packet->header.sequence != sequence) continue;
    protocol::AckPayload ack{};
    if (!protocol::decode_payload(*packet, ack)) return false;
    return ack.command_type == command_type && ack.status == static_cast<uint8_t>(protocol::AckStatus::kAccepted);
  }
  return false;
}

bool MobileRobotSystem::read_one_frame(std::vector<uint8_t>& frame, std::chrono::milliseconds timeout) {
  frame.clear();
  if (serial_fd_ < 0) return false;
  auto read_exact = [&](uint8_t* dst, std::size_t len) {
    std::size_t got = 0;
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (got < len) {
      const auto now = std::chrono::steady_clock::now();
      if (now >= deadline) return false;
      const auto wait = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now);
      pollfd d{serial_fd_, POLLIN, 0};
      if (::poll(&d, 1, static_cast<int>(wait.count())) <= 0) return false;
      const ssize_t n = ::read(serial_fd_, dst + got, len - got);
      if (n > 0) got += static_cast<std::size_t>(n);
    }
    return true;
  };
  protocol::Header h{};
  if (!read_exact(reinterpret_cast<uint8_t*>(&h), sizeof(h))) return false;
  if (h.magic != protocol::kMagic || h.version != protocol::kVersion || h.payload_size > protocol::kMaximumPayloadSize) { tcflush(serial_fd_, TCIFLUSH); return false; }
  frame.resize(sizeof(h) + h.payload_size + sizeof(uint16_t));
  std::memcpy(frame.data(), &h, sizeof(h));
  if (!read_exact(frame.data() + sizeof(h), h.payload_size + sizeof(uint16_t))) { frame.clear(); return false; }
  return true;
}

bool MobileRobotSystem::write_all(const std::vector<uint8_t>& data) {
  if (serial_fd_ < 0) return false;
  std::size_t written = 0;
  while (written < data.size()) {
    const ssize_t n = ::write(serial_fd_, data.data() + written, data.size() - written);
    if (n < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        pollfd d{serial_fd_, POLLOUT, 0};
        if (::poll(&d, 1, 100) <= 0) return false;
        continue;
      }
      return false;
    }
    written += static_cast<std::size_t>(n);
  }
  return true;
}

void MobileRobotSystem::start_diagnostics() {
  const auto node = get_node();
  if (!node) {
    RCLCPP_WARN(get_logger(), "Hardware framework-managed node unavailable; diagnostics disabled");
    return;
  }
  diagnostics_publisher_ = node->create_publisher<diagnostic_msgs::msg::DiagnosticArray>("/diagnostics", rclcpp::QoS(10));
  diagnostics_timer_ = node->create_wall_timer(kDiagnosticsPeriod, [this]() { publish_hardware_diagnostics(); });
}

void MobileRobotSystem::stop_diagnostics() {
  diagnostics_timer_.reset();
  diagnostics_publisher_.reset();
}

void MobileRobotSystem::publish_hardware_diagnostics() {
  if (!diagnostics_publisher_) return;
  const int64_t now_ns = steady_now_nanoseconds();
  const int64_t last_ns = diagnostic_last_telemetry_ns_.load(std::memory_order_relaxed);
  const bool configured = diagnostic_configured_.load(std::memory_order_relaxed);
  const bool enabled = diagnostic_enabled_.load(std::memory_order_relaxed);
  const uint32_t fault_flags = diagnostic_fault_flags_.load(std::memory_order_relaxed);
  double telemetry_age_ms = -1.0;
  if (last_ns > 0) telemetry_age_ms = static_cast<double>(now_ns - last_ns) / 1'000'000.0;

  diagnostic_msgs::msg::DiagnosticStatus status;
  status.name = "Arty-ROS2 ESP32 base hardware";
  status.hardware_id = "esp32_base_controller";
  if (!configured) {
    status.level = diagnostic_msgs::msg::DiagnosticStatus::ERROR;
    status.message = "ESP32 is not configured";
  } else if (telemetry_age_ms < 0.0 || telemetry_age_ms > static_cast<double>(telemetry_timeout_ms_)) {
    status.level = diagnostic_msgs::msg::DiagnosticStatus::ERROR;
    status.message = "ESP32 telemetry timeout";
  } else if (fault_flags != 0U) {
    status.level = diagnostic_msgs::msg::DiagnosticStatus::ERROR;
    status.message = "ESP32 reported fault flags";
  } else {
    status.level = diagnostic_msgs::msg::DiagnosticStatus::OK;
    status.message = enabled ? "Configured, enabled, telemetry healthy" : "Configured, disabled, telemetry healthy";
  }

  status.values = {
    key_value("serial_device", serial_device_),
    key_value("baud_rate", std::to_string(baud_rate_)),
    key_value("ticks_per_revolution", std::to_string(ticks_per_revolution_)),
    key_value("telemetry_timeout_ms", std::to_string(telemetry_timeout_ms_)),
    key_value("telemetry_age_ms", telemetry_age_ms < 0.0 ? "never" : std::to_string(telemetry_age_ms)),
    key_value("configured", configured ? "true" : "false"),
    key_value("enabled", enabled ? "true" : "false"),
    key_value("fault_flags", std::to_string(fault_flags)),
    key_value("left_ticks", std::to_string(diagnostic_left_ticks_.load(std::memory_order_relaxed))),
    key_value("right_ticks", std::to_string(diagnostic_right_ticks_.load(std::memory_order_relaxed))),
    key_value("left_velocity_rad_per_sec", std::to_string(diagnostic_left_velocity_.load(std::memory_order_relaxed))),
    key_value("right_velocity_rad_per_sec", std::to_string(diagnostic_right_velocity_.load(std::memory_order_relaxed)))
  };

  diagnostic_msgs::msg::DiagnosticArray message;
  const auto node = get_node();
  if (node) message.header.stamp = node->get_clock()->now().to_msg();
  message.status.push_back(std::move(status));
  diagnostics_publisher_->publish(message);
}

}  // namespace mobile_robot_hardware

PLUGINLIB_EXPORT_CLASS(mobile_robot_hardware::MobileRobotSystem, hardware_interface::SystemInterface)
