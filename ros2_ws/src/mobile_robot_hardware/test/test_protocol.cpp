#include <gtest/gtest.h>

#include <bit>
#include <cstdint>
#include <optional>
#include <vector>

#include "mobile_robot_hardware/protocol.hpp"

namespace protocol =
    mobile_robot_hardware::protocol;

namespace {

constexpr uint32_t
    kSerializationSentinelBits =
        0x3F400000U;

const float kSerializationSentinel =
    std::bit_cast<float>(
        kSerializationSentinelBits);

}  // namespace

TEST(
    Protocol,
    ConfigureRoundTrip) {
  const protocol::ConfigurePayload payload{
      kSerializationSentinel,
      0U,
      1U,
      0U,
      1U,
  };

  const auto frame =
      protocol::encode_frame(
          protocol::MessageType::kCommandConfigure,
          42U,
          payload);

  const auto decoded =
      protocol::decode_frame(frame);

  ASSERT_TRUE(decoded.has_value());
  EXPECT_EQ(
      decoded->header.sequence,
      42U);

  protocol::ConfigurePayload output{};

  ASSERT_TRUE(
      protocol::decode_payload(
          *decoded,
          output));

  EXPECT_EQ(
      std::bit_cast<uint32_t>(
          output.ticks_per_revolution),
      kSerializationSentinelBits);

  EXPECT_EQ(
      output.left_motor_inverted,
      0U);

  EXPECT_EQ(
      output.right_motor_inverted,
      1U);

  EXPECT_EQ(
      output.left_encoder_inverted,
      0U);

  EXPECT_EQ(
      output.right_encoder_inverted,
      1U);
}

TEST(
    Protocol,
    AcceptedAckCompatibilityAliasIsStable) {
  EXPECT_EQ(
      static_cast<uint8_t>(
          protocol::AckStatus::kAccepted),
      0U);

  EXPECT_EQ(
      static_cast<uint8_t>(
          protocol::AckStatus::kOk),
      static_cast<uint8_t>(
          protocol::AckStatus::kAccepted));
}

TEST(
    Protocol,
    RejectsCorruptedCrc) {
  protocol::EnablePayload payload{
      1U,
  };

  auto frame =
      protocol::encode_frame(
          protocol::MessageType::kCommandEnable,
          7U,
          payload);

  frame[sizeof(protocol::Header)] ^=
      0x01U;

  EXPECT_FALSE(
      protocol::decode_frame(
          frame)
          .has_value());
}

TEST(
    Protocol,
    AckRoundTrip) {
  const protocol::AckPayload payload{
      static_cast<uint8_t>(
          protocol::MessageType::
              kCommandConfigure),
      static_cast<uint8_t>(
          protocol::AckStatus::
              kAccepted),
  };

  const auto frame =
      protocol::encode_frame(
          protocol::MessageType::kAck,
          99U,
          payload);

  const auto decoded =
      protocol::decode_frame(frame);

  ASSERT_TRUE(decoded.has_value());

  protocol::AckPayload ack{};

  ASSERT_TRUE(
      protocol::decode_payload(
          *decoded,
          ack));

  EXPECT_EQ(
      ack.command_type,
      static_cast<uint8_t>(
          protocol::MessageType::
              kCommandConfigure));

  EXPECT_EQ(
      ack.status,
      static_cast<uint8_t>(
          protocol::AckStatus::
              kAccepted));
}

TEST(
    FrameStreamParser,
    RecoversFromGarbagePrefix) {
  const protocol::HeartbeatPayload payload{
      0U,
  };

  const auto frame =
      protocol::encode_frame(
          protocol::MessageType::kHeartbeat,
          10U,
          payload);

  const std::vector<uint8_t> stream{
      0x00U,
      0xFFU,
      0x13U,
      0x5AU,
      0x00U,
      0xA5U,
  };

  protocol::FrameStreamParser parser;
  std::optional<std::vector<uint8_t>> output;

  for (const uint8_t byte : stream) {
    output = parser.push(byte);
    EXPECT_FALSE(output.has_value());
  }

  for (const uint8_t byte : frame) {
    const auto result =
        parser.push(byte);

    if (result.has_value()) {
      output = result;
    }
  }

  ASSERT_TRUE(output.has_value());
  EXPECT_EQ(*output, frame);
}

TEST(
    FrameStreamParser,
    SupportsFragmentedFrame) {
  const protocol::VelocityPayload payload{
      1.25F,
      -0.75F,
  };

  const auto frame =
      protocol::encode_frame(
          protocol::MessageType::kCommandVelocity,
          22U,
          payload);

  protocol::FrameStreamParser parser;
  std::optional<std::vector<uint8_t>> output;

  for (std::size_t index = 0U;
       index < frame.size();
       ++index) {
    const auto result =
        parser.push(frame[index]);

    if (index + 1U < frame.size()) {
      EXPECT_FALSE(result.has_value());
    } else {
      output = result;
    }
  }

  ASSERT_TRUE(output.has_value());
  EXPECT_EQ(*output, frame);
}

TEST(
    FrameStreamParser,
    RecoversAfterBadCrcThenValidFrame) {
  protocol::EnablePayload bad_payload{
      1U,
  };

  auto bad_frame =
      protocol::encode_frame(
          protocol::MessageType::kCommandEnable,
          30U,
          bad_payload);

  bad_frame.back() ^= 0x80U;

  const protocol::AckPayload good_payload{
      static_cast<uint8_t>(
          protocol::MessageType::
              kCommandEnable),
      static_cast<uint8_t>(
          protocol::AckStatus::
              kAccepted),
  };

  const auto good_frame =
      protocol::encode_frame(
          protocol::MessageType::kAck,
          31U,
          good_payload);

  protocol::FrameStreamParser parser;
  std::optional<std::vector<uint8_t>> output;

  for (const uint8_t byte : bad_frame) {
    EXPECT_FALSE(
        parser.push(byte).has_value());
  }

  for (const uint8_t byte : good_frame) {
    const auto result =
        parser.push(byte);

    if (result.has_value()) {
      output = result;
    }
  }

  ASSERT_TRUE(output.has_value());
  EXPECT_EQ(*output, good_frame);
}
