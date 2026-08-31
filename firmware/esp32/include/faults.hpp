#pragma once
#include <cstdint>

namespace mobile_robot::faults {

enum FaultFlag : uint32_t {
  kNone = 0U,
  kCommandTimeout = 1U << 0U,
  kHeartbeatTimeout = 1U << 1U,
  kInvalidConfiguration = 1U << 2U,
  kProtocolError = 1U << 3U,
  kInvalidVelocityCommand = 1U << 4U,
};

inline void set(uint32_t& flags, const FaultFlag flag) {
  flags |= static_cast<uint32_t>(flag);
}

inline void clear(uint32_t& flags, const FaultFlag flag) {
  flags &= ~static_cast<uint32_t>(flag);
}

inline void clear_all(uint32_t& flags) {
  flags = kNone;
}

}  // namespace mobile_robot::faults
