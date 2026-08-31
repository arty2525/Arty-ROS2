#include <gtest/gtest.h>

#include <cstdint>

#include "mobile_robot_hardware/fault_flags.hpp"

namespace faults = mobile_robot_hardware::faults;

TEST(FaultFlags, MatchesFirmwareBitLayout) {
  EXPECT_EQ(faults::mask(faults::FaultFlag::kCommandTimeout), 1U << 0U);
  EXPECT_EQ(faults::mask(faults::FaultFlag::kHeartbeatTimeout), 1U << 1U);
  EXPECT_EQ(faults::mask(faults::FaultFlag::kInvalidConfiguration), 1U << 2U);
  EXPECT_EQ(faults::mask(faults::FaultFlag::kProtocolError), 1U << 3U);
  EXPECT_EQ(faults::mask(faults::FaultFlag::kInvalidVelocityCommand), 1U << 4U);
}

TEST(FaultFlags, DecodesMultipleKnownFaults) {
  const uint32_t flags =
      faults::mask(faults::FaultFlag::kCommandTimeout) |
      faults::mask(faults::FaultFlag::kProtocolError);

  EXPECT_TRUE(faults::is_set(flags, faults::FaultFlag::kCommandTimeout));
  EXPECT_TRUE(faults::is_set(flags, faults::FaultFlag::kProtocolError));
  EXPECT_FALSE(faults::is_set(flags, faults::FaultFlag::kHeartbeatTimeout));
  EXPECT_EQ(faults::unknown_bits(flags), 0U);
  EXPECT_EQ(faults::active_faults_text(flags), "command timeout, protocol error");
}

TEST(FaultFlags, PreservesUnknownBits) {
  constexpr uint32_t unknown = 1U << 31U;
  EXPECT_EQ(faults::unknown_bits(unknown), unknown);
  EXPECT_EQ(faults::active_faults_text(unknown), "unknown fault bits");
}

TEST(FaultFlags, ReportsNoneForZero) {
  EXPECT_EQ(faults::active_faults_text(0U), "none");
}
