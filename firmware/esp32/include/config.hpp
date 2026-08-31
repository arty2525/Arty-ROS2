#pragma once

#include <Arduino.h>

namespace mobile_robot::config {
constexpr uint32_t kUsbSerialBaud = 115200;
constexpr uint32_t kControlSerialBaud = 115200;
constexpr int kUartRxPin = 16;
constexpr int kUartTxPin = 17;
constexpr int kLeftPwmPin = 25;
constexpr int kLeftDirPin = 26;
constexpr int kRightPwmPin = 27;
constexpr int kRightDirPin = 14;
constexpr int kLeftEncoderAPin = 32;
constexpr int kLeftEncoderBPin = 33;
constexpr int kRightEncoderAPin = 34;
constexpr int kRightEncoderBPin = 35;
constexpr uint8_t kLeftPwmChannel = 0;
constexpr uint8_t kRightPwmChannel = 1;
constexpr uint32_t kPwmFrequencyHz = 20000;
constexpr uint8_t kPwmResolutionBits = 10;
constexpr uint16_t kPwmMax = (1U << kPwmResolutionBits) - 1U;
constexpr uint32_t kControlPeriodUs = 10000;
constexpr uint32_t kTelemetryPeriodUs = 20000;
constexpr uint32_t kCommandTimeoutMs = 250;
constexpr uint32_t kHeartbeatTimeoutMs = 1000;
constexpr float kMaxTargetRadPerSec = 30.0F;
constexpr float kDefaultKp = 0.12F;
constexpr float kDefaultKi = 0.30F;
constexpr float kDefaultKd = 0.001F;
constexpr float kDefaultFeedForward = 0.03F;
constexpr float kIntegratorLimit = 0.60F;
constexpr uint16_t kProtocolMagic = 0xA55A;
constexpr uint8_t kProtocolVersion = 1;
constexpr size_t kMaximumPayloadSize = 64;
}  // namespace mobile_robot::config
