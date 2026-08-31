#pragma once

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>
#include <vector>

#include "diagnostic_msgs/msg/diagnostic_array.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "mobile_robot_hardware/protocol.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/timer.hpp"
#include "rclcpp_lifecycle/state.hpp"

namespace mobile_robot_hardware {

class MobileRobotSystem
    : public hardware_interface::SystemInterface {
 public:
  RCLCPP_SHARED_PTR_DEFINITIONS(
      MobileRobotSystem)

  hardware_interface::CallbackReturn
  on_init(
      const hardware_interface::HardwareInfo&
          info) override;

  hardware_interface::CallbackReturn
  on_configure(
      const rclcpp_lifecycle::State&) override;

  hardware_interface::CallbackReturn
  on_activate(
      const rclcpp_lifecycle::State&) override;

  hardware_interface::CallbackReturn
  on_deactivate(
      const rclcpp_lifecycle::State&) override;

  hardware_interface::CallbackReturn
  on_cleanup(
      const rclcpp_lifecycle::State&) override;

  std::vector<hardware_interface::StateInterface>
  export_state_interfaces() override;

  std::vector<hardware_interface::CommandInterface>
  export_command_interfaces() override;

  hardware_interface::return_type
  read(
      const rclcpp::Time&,
      const rclcpp::Duration&) override;

  hardware_interface::return_type
  write(
      const rclcpp::Time&,
      const rclcpp::Duration&) override;

 private:
  bool open_serial();
  void close_serial();

  bool configure_firmware();
  bool clear_firmware_faults();
  bool set_firmware_enabled(bool enabled);

  bool send_velocity();
  bool send_heartbeat();

  bool wait_for_ack(
      uint16_t sequence,
      uint8_t command_type,
      std::chrono::milliseconds timeout);

  bool read_serial_frame(
      std::vector<uint8_t>& frame,
      std::chrono::milliseconds timeout);

  bool read_one_frame(
      std::vector<uint8_t>& frame,
      std::chrono::milliseconds timeout);

  bool write_all(
      const std::vector<uint8_t>& data);

  void setup_diagnostics();
  void publish_hardware_diagnostics();

  static int64_t steady_now_nanoseconds();

  std::string serial_device_;
  int baud_rate_{115200};
  double ticks_per_revolution_{0.0};
  int telemetry_timeout_ms_{500};

  bool left_motor_inverted_{false};
  bool right_motor_inverted_{false};
  bool left_encoder_inverted_{false};
  bool right_encoder_inverted_{false};

  std::size_t left_joint_index_{0U};
  std::size_t right_joint_index_{0U};

  int serial_fd_{-1};
  uint16_t sequence_{1};

  protocol::FrameStreamParser
      serial_parser_{};

  std::deque<std::vector<uint8_t>>
      pending_frames_{};

  double left_position_{0.0};
  double right_position_{0.0};
  double left_velocity_{0.0};
  double right_velocity_{0.0};
  double left_command_{0.0};
  double right_command_{0.0};

  uint32_t last_fault_flags_{0U};
  bool have_received_telemetry_{false};

  std::chrono::steady_clock::time_point
      last_heartbeat_{};

  std::chrono::steady_clock::time_point
      last_telemetry_{};

  rclcpp::Publisher<
      diagnostic_msgs::msg::DiagnosticArray
    >::SharedPtr diagnostics_publisher_;

  rclcpp::TimerBase::SharedPtr
      diagnostics_timer_;

  std::atomic<bool>
      diagnostic_configured_{false};

  std::atomic<bool>
      diagnostic_enabled_{false};

  std::atomic<bool>
      diagnostic_have_telemetry_{false};

  std::atomic<uint32_t>
      diagnostic_fault_flags_{0U};

  std::atomic<int64_t>
      diagnostic_last_telemetry_ns_{0};

  std::atomic<int64_t>
      diagnostic_left_ticks_{0};

  std::atomic<int64_t>
      diagnostic_right_ticks_{0};

  std::atomic<double>
      diagnostic_left_velocity_{0.0};

  std::atomic<double>
      diagnostic_right_velocity_{0.0};
};

}  // namespace mobile_robot_hardware
