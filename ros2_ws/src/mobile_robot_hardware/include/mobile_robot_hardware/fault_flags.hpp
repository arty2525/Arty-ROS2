#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace mobile_robot_hardware::faults {

enum class FaultFlag : uint32_t {
  kCommandTimeout = 1U << 0U,
  kHeartbeatTimeout = 1U << 1U,
  kInvalidConfiguration = 1U << 2U,
  kProtocolError = 1U << 3U,
  kInvalidVelocityCommand = 1U << 4U,
};

struct FaultDescription {
  FaultFlag flag;
  std::string_view key;
  std::string_view text;
};

constexpr std::array<FaultDescription, 5U> kKnownFaults{{
    {FaultFlag::kCommandTimeout, "command_timeout", "command timeout"},
    {FaultFlag::kHeartbeatTimeout, "heartbeat_timeout", "heartbeat timeout"},
    {FaultFlag::kInvalidConfiguration, "invalid_configuration", "invalid configuration"},
    {FaultFlag::kProtocolError, "protocol_error", "protocol error"},
    {FaultFlag::kInvalidVelocityCommand, "invalid_velocity_command", "invalid velocity command"},
}};

constexpr uint32_t mask(const FaultFlag flag) {
  return static_cast<uint32_t>(flag);
}

constexpr bool is_set(const uint32_t flags, const FaultFlag flag) {
  return (flags & mask(flag)) != 0U;
}

constexpr uint32_t known_mask() {
  uint32_t result = 0U;
  for (const auto& fault : kKnownFaults) {
    result |= mask(fault.flag);
  }
  return result;
}

constexpr uint32_t unknown_bits(const uint32_t flags) {
  return flags & ~known_mask();
}

inline std::string active_faults_text(const uint32_t flags) {
  if (flags == 0U) {
    return "none";
  }

  std::string result;
  for (const auto& fault : kKnownFaults) {
    if (!is_set(flags, fault.flag)) {
      continue;
    }
    if (!result.empty()) {
      result += ", ";
    }
    result += fault.text;
  }

  if (unknown_bits(flags) != 0U) {
    if (!result.empty()) {
      result += ", ";
    }
    result += "unknown fault bits";
  }

  return result;
}

}  // namespace mobile_robot_hardware::faults
