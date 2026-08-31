#include <Arduino.h>

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "config.hpp"
#include "encoder_reader.hpp"
#include "faults.hpp"
#include "motor_driver.hpp"
#include "protocol.hpp"
#include "velocity_controller.hpp"

namespace {

using namespace mobile_robot;
using namespace mobile_robot::protocol;
using mobile_robot::faults::FaultFlag;

HardwareSerial control_serial(2);

MotorDriver left_motor(
    config::kLeftPwmPin,
    config::kLeftDirPin,
    config::kLeftPwmChannel,
    false);

MotorDriver right_motor(
    config::kRightPwmPin,
    config::kRightDirPin,
    config::kRightPwmChannel,
    true);

EncoderReader left_encoder(
    config::kLeftEncoderAPin,
    config::kLeftEncoderBPin,
    false);

EncoderReader right_encoder(
    config::kRightEncoderAPin,
    config::kRightEncoderBPin,
    true);

VelocityController left_controller;
VelocityController right_controller;
Parser parser;

float ticks_per_revolution = 0.0F;
float left_target = 0.0F;
float right_target = 0.0F;
float left_velocity = 0.0F;
float right_velocity = 0.0F;

int64_t previous_left_ticks = 0;
int64_t previous_right_ticks = 0;

bool configured = false;
bool enabled = false;

uint32_t fault_flags = 0U;
uint32_t last_command_ms = 0U;
uint32_t last_heartbeat_ms = 0U;
uint32_t last_control_us = 0U;
uint32_t last_telemetry_us = 0U;

uint16_t telemetry_sequence = 0U;

template <typename T>
void send_packet(
    const MessageType type,
    const uint16_t sequence,
    const T& payload) {
  uint8_t frame[
      sizeof(Header) +
      config::kMaximumPayloadSize +
      sizeof(uint16_t)]{};

  const size_t size = encode_packet(
      type,
      sequence,
      payload,
      frame,
      sizeof(frame));

  if (size > 0U) {
    control_serial.write(frame, size);
  }
}

void send_ack(
    const Packet& packet,
    const AckStatus status) {
  const AckPayload payload{
      packet.header.type,
      static_cast<uint8_t>(status),
  };

  send_packet(
      MessageType::kAck,
      packet.header.sequence,
      payload);
}

void stop_motion() {
  enabled = false;
  left_target = 0.0F;
  right_target = 0.0F;

  left_controller.reset();
  right_controller.reset();

  left_motor.stop();
  right_motor.stop();
}

bool valid_positive_finite(const float value) {
  return std::isfinite(value) && value > 0.0F;
}

void handle_packet(const Packet& packet) {
  const auto type =
      static_cast<MessageType>(packet.header.type);

  switch (type) {
    case MessageType::kCommandConfigure: {
      ConfigurePayload payload{};

      if (!decode_payload(packet, payload)) {
        faults::set(
            fault_flags,
            FaultFlag::kProtocolError);
        send_ack(
            packet,
            AckStatus::kInvalidPayload);
        return;
      }

      if (!valid_positive_finite(
              payload.ticks_per_revolution)) {
        stop_motion();
        configured = false;
        ticks_per_revolution = 0.0F;

        faults::set(
            fault_flags,
            FaultFlag::kInvalidConfiguration);

        send_ack(
            packet,
            AckStatus::kInvalidConfiguration);
        return;
      }

      stop_motion();

      ticks_per_revolution =
          payload.ticks_per_revolution;

      left_encoder.reset();
      right_encoder.reset();

      previous_left_ticks = 0;
      previous_right_ticks = 0;
      left_velocity = 0.0F;
      right_velocity = 0.0F;

      configured = true;

      faults::clear(
          fault_flags,
          FaultFlag::kInvalidConfiguration);
      faults::clear(
          fault_flags,
          FaultFlag::kCommandTimeout);
      faults::clear(
          fault_flags,
          FaultFlag::kHeartbeatTimeout);

      last_command_ms = millis();
      last_heartbeat_ms = millis();

      send_ack(
          packet,
          AckStatus::kAccepted);
      return;
    }

    case MessageType::kCommandEnable: {
      EnablePayload payload{};

      if (!decode_payload(packet, payload)) {
        faults::set(
            fault_flags,
            FaultFlag::kProtocolError);
        send_ack(
            packet,
            AckStatus::kInvalidPayload);
        return;
      }

      if (payload.enabled != 0U &&
          !configured) {
        stop_motion();
        send_ack(
            packet,
            AckStatus::kRejectedByState);
        return;
      }

      enabled = payload.enabled != 0U;

      if (!enabled) {
        stop_motion();
      }

      last_command_ms = millis();

      send_ack(
          packet,
          AckStatus::kAccepted);
      return;
    }

    case MessageType::kCommandVelocity: {
      VelocityPayload payload{};

      if (!decode_payload(packet, payload)) {
        faults::set(
            fault_flags,
            FaultFlag::kProtocolError);
        return;
      }

      if (!std::isfinite(
              payload.left_rad_per_sec) ||
          !std::isfinite(
              payload.right_rad_per_sec)) {
        faults::set(
            fault_flags,
            FaultFlag::kInvalidVelocityCommand);
        stop_motion();
        return;
      }

      if (!configured || !enabled) {
        return;
      }

      faults::clear(
          fault_flags,
          FaultFlag::kInvalidVelocityCommand);
      faults::clear(
          fault_flags,
          FaultFlag::kCommandTimeout);

      left_target = std::clamp(
          payload.left_rad_per_sec,
          -config::kMaxTargetRadPerSec,
          config::kMaxTargetRadPerSec);

      right_target = std::clamp(
          payload.right_rad_per_sec,
          -config::kMaxTargetRadPerSec,
          config::kMaxTargetRadPerSec);

      last_command_ms = millis();
      return;
    }

    case MessageType::kCommandSetPid: {
      PidPayload payload{};

      if (!decode_payload(packet, payload)) {
        faults::set(
            fault_flags,
            FaultFlag::kProtocolError);
        send_ack(
            packet,
            AckStatus::kInvalidPayload);
        return;
      }

      if (!std::isfinite(payload.kp) ||
          !std::isfinite(payload.ki) ||
          !std::isfinite(payload.kd) ||
          !std::isfinite(payload.feed_forward)) {
        faults::set(
            fault_flags,
            FaultFlag::kInvalidConfiguration);
        send_ack(
            packet,
            AckStatus::kInvalidConfiguration);
        return;
      }

      left_controller.configure(
          payload.kp,
          payload.ki,
          payload.kd,
          payload.feed_forward,
          config::kIntegratorLimit);

      right_controller.configure(
          payload.kp,
          payload.ki,
          payload.kd,
          payload.feed_forward,
          config::kIntegratorLimit);

      send_ack(
          packet,
          AckStatus::kAccepted);
      return;
    }

    case MessageType::kCommandClearFaults:
      faults::clear_all(fault_flags);
      send_ack(
          packet,
          AckStatus::kAccepted);
      return;

    case MessageType::kHeartbeat: {
      HeartbeatPayload payload{};

      if (!decode_payload(packet, payload)) {
        faults::set(
            fault_flags,
            FaultFlag::kProtocolError);
        return;
      }

      last_heartbeat_ms = millis();

      faults::clear(
          fault_flags,
          FaultFlag::kHeartbeatTimeout);

      return;
    }

    default:
      faults::set(
          fault_flags,
          FaultFlag::kProtocolError);

      send_ack(
          packet,
          AckStatus::kUnsupported);
      return;
  }
}

void process_serial() {
  Packet packet{};

  while (control_serial.available() > 0) {
    const int value = control_serial.read();

    if (value >= 0 &&
        parser.push(
            static_cast<uint8_t>(value),
            packet)) {
      handle_packet(packet);
    }
  }
}

void update_control() {
  const uint32_t now_us = micros();

  if (now_us - last_control_us <
      config::kControlPeriodUs) {
    return;
  }

  const float dt =
      static_cast<float>(
          now_us - last_control_us) /
      1'000'000.0F;

  last_control_us = now_us;

  const int64_t left_ticks =
      left_encoder.ticks();
  const int64_t right_ticks =
      right_encoder.ticks();

  if (configured &&
      ticks_per_revolution > 0.0F) {
    constexpr float kTwoPi =
        6.28318530717958647692F;

    left_velocity =
        static_cast<float>(
            left_ticks - previous_left_ticks) *
        kTwoPi /
        ticks_per_revolution /
        dt;

    right_velocity =
        static_cast<float>(
            right_ticks - previous_right_ticks) *
        kTwoPi /
        ticks_per_revolution /
        dt;
  } else {
    left_velocity = 0.0F;
    right_velocity = 0.0F;
  }

  previous_left_ticks = left_ticks;
  previous_right_ticks = right_ticks;

  const uint32_t now_ms = millis();

  if (enabled &&
      now_ms - last_command_ms >
          config::kCommandTimeoutMs) {
    faults::set(
        fault_flags,
        FaultFlag::kCommandTimeout);
    stop_motion();
  }

  if (now_ms - last_heartbeat_ms >
      config::kHeartbeatTimeoutMs) {
    faults::set(
        fault_flags,
        FaultFlag::kHeartbeatTimeout);
    stop_motion();
  }

  if (!configured || !enabled) {
    left_motor.stop();
    right_motor.stop();
    return;
  }

  left_motor.set(
      left_controller.update(
          left_target,
          left_velocity,
          dt));

  right_motor.set(
      right_controller.update(
          right_target,
          right_velocity,
          dt));
}

void publish_telemetry() {
  const uint32_t now_us = micros();

  if (now_us - last_telemetry_us <
      config::kTelemetryPeriodUs) {
    return;
  }

  last_telemetry_us = now_us;

  const TelemetryPayload payload{
      left_encoder.ticks(),
      right_encoder.ticks(),
      left_velocity,
      right_velocity,
      fault_flags,
      static_cast<uint8_t>(
          enabled ? 1U : 0U),
      static_cast<uint8_t>(
          configured ? 1U : 0U),
  };

  send_packet(
      MessageType::kTelemetry,
      telemetry_sequence++,
      payload);
}

}  // namespace

void setup() {
  Serial.begin(
      mobile_robot::config::kUsbSerialBaud);

  control_serial.begin(
      mobile_robot::config::kControlSerialBaud,
      SERIAL_8N1,
      mobile_robot::config::kUartRxPin,
      mobile_robot::config::kUartTxPin);

  left_motor.begin(
      mobile_robot::config::kPwmFrequencyHz,
      mobile_robot::config::kPwmResolutionBits);

  right_motor.begin(
      mobile_robot::config::kPwmFrequencyHz,
      mobile_robot::config::kPwmResolutionBits);

  left_controller.configure(
      mobile_robot::config::kDefaultKp,
      mobile_robot::config::kDefaultKi,
      mobile_robot::config::kDefaultKd,
      mobile_robot::config::kDefaultFeedForward,
      mobile_robot::config::kIntegratorLimit);

  right_controller.configure(
      mobile_robot::config::kDefaultKp,
      mobile_robot::config::kDefaultKi,
      mobile_robot::config::kDefaultKd,
      mobile_robot::config::kDefaultFeedForward,
      mobile_robot::config::kIntegratorLimit);

  stop_motion();

  last_command_ms = millis();
  last_heartbeat_ms = millis();
  last_control_us = micros();
  last_telemetry_us = micros();
}

void loop() {
  process_serial();
  update_control();
  publish_telemetry();
}
