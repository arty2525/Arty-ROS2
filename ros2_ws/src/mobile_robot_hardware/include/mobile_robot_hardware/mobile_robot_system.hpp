#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "diagnostic_msgs/msg/diagnostic_array.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/timer.hpp"
#include "rclcpp_lifecycle/state.hpp"

namespace mobile_robot_hardware {

class MobileRobotSystem : public hardware_interface::SystemInterface {
 public:
  RCLCPP_SHARED_PTR_DEFINITIONS(MobileRobotSystem)

  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;
  hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State&) override;
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State&) override;
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State&) override;
  hardware_interface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State&) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  hardware_interface::return_type read(const rclcpp::Time&, const rclcpp::Duration&) override;
  hardware_interface::return_type write(const rclcpp::Time&, const rclcpp::Duration&) override;

 private:
  bool open_serial();
  void close_serial();
  bool configure_firmware();
  bool set_firmware_enabled(bool enabled);
  bool send_velocity();
  bool send_heartbeat();
  bool wait_for_ack(uint16_t sequence, uint8_t command_type, std::chrono::milliseconds timeout);
  bool read_one_frame(std::vector<uint8_t>& frame, std::chrono::milliseconds timeout);
  bool write_all(const std::vector<uint8_t>& data);
  void start_diagnostics();
  void stop_diagnostics();
  void publish_hardware_diagnostics();

  std::string serial_device_;
  int baud_rate_{115200};
  double ticks_per_revolution_{0.0};
  int telemetry_timeout_ms_{500};
  int serial_fd_{-1};
  uint16_t sequence_{1};

  double left_position_{0.0};
  double right_position_{0.0};
  double left_velocity_{0.0};
  double right_velocity_{0.0};
  double left_command_{0.0};
  double right_command_{0.0};
  std::chrono::steady_clock::time_point last_heartbeat_{};

  std::atomic<int64_t> diagnostic_last_telemetry_ns_{0};
  std::atomic<int64_t> diagnostic_left_ticks_{0};
  std::atomic<int64_t> diagnostic_right_ticks_{0};
  std::atomic<double> diagnostic_left_velocity_{0.0};
  std::atomic<double> diagnostic_right_velocity_{0.0};
  std::atomic<uint32_t> diagnostic_fault_flags_{0U};
  std::atomic<bool> diagnostic_configured_{false};
  std::atomic<bool> diagnostic_enabled_{false};

  rclcpp::Publisher<diagnostic_msgs::msg::DiagnosticArray>::SharedPtr diagnostics_publisher_;
  rclcpp::TimerBase::SharedPtr diagnostics_timer_;
};

}  // namespace mobile_robot_hardware
