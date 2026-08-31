#include "mobile_robot_hardware/protocol.hpp"

namespace mobile_robot_hardware::protocol {

namespace {

constexpr uint8_t kMagicLow =
    static_cast<uint8_t>(
        kMagic & 0x00FFU);

constexpr uint8_t kMagicHigh =
    static_cast<uint8_t>(
        (kMagic >> 8U) & 0x00FFU);

}  // namespace

uint16_t crc16_ccitt_false(
    const uint8_t* data,
    const std::size_t size) {
  uint16_t crc = 0xFFFFU;

  for (std::size_t index = 0U;
       index < size;
       ++index) {
    crc ^=
        static_cast<uint16_t>(
            data[index]) << 8U;

    for (uint8_t bit = 0U;
         bit < 8U;
         ++bit) {
      crc =
          (crc & 0x8000U) != 0U
              ? static_cast<uint16_t>(
                    (crc << 1U) ^ 0x1021U)
              : static_cast<uint16_t>(
                    crc << 1U);
    }
  }

  return crc;
}

std::optional<Packet> decode_frame(
    const std::vector<uint8_t>& frame) {
  if (
      frame.size() <
      sizeof(Header) + sizeof(uint16_t)) {
    return std::nullopt;
  }

  Packet packet{};

  std::memcpy(
      &packet.header,
      frame.data(),
      sizeof(Header));

  if (
      packet.header.magic != kMagic ||
      packet.header.version != kVersion ||
      packet.header.payload_size >
          kMaximumPayloadSize) {
    return std::nullopt;
  }

  const std::size_t expected_size =
      sizeof(Header) +
      packet.header.payload_size +
      sizeof(uint16_t);

  if (frame.size() != expected_size) {
    return std::nullopt;
  }

  std::memcpy(
      packet.payload.data(),
      frame.data() + sizeof(Header),
      packet.header.payload_size);

  std::memcpy(
      &packet.crc,
      frame.data() +
          sizeof(Header) +
          packet.header.payload_size,
      sizeof(packet.crc));

  const uint16_t calculated_crc =
      crc16_ccitt_false(
          frame.data(),
          sizeof(Header) +
              packet.header.payload_size);

  if (calculated_crc != packet.crc) {
    return std::nullopt;
  }

  return packet;
}

void FrameStreamParser::reset() {
  state_ = State::kMagicLow;
  candidate_.clear();
  header_ = Header{};
  expected_frame_size_ = 0U;
}

void FrameStreamParser::begin_candidate() {
  candidate_.clear();
  candidate_.push_back(kMagicLow);
  state_ = State::kMagicHigh;
}

void FrameStreamParser::recover_from_byte(
    const uint8_t byte) {
  reset();

  if (byte == kMagicLow) {
    begin_candidate();
  }
}

std::optional<std::vector<uint8_t>>
FrameStreamParser::push(
    const uint8_t byte) {
  switch (state_) {
    case State::kMagicLow:
      if (byte == kMagicLow) {
        begin_candidate();
      }
      return std::nullopt;

    case State::kMagicHigh:
      if (byte == kMagicHigh) {
        candidate_.push_back(byte);
        state_ = State::kHeaderRest;
        return std::nullopt;
      }

      if (byte == kMagicLow) {
        begin_candidate();
        return std::nullopt;
      }

      reset();
      return std::nullopt;

    case State::kHeaderRest:
      candidate_.push_back(byte);

      if (candidate_.size() < sizeof(Header)) {
        return std::nullopt;
      }

      std::memcpy(
          &header_,
          candidate_.data(),
          sizeof(Header));

      if (
          header_.magic != kMagic ||
          header_.version != kVersion ||
          header_.payload_size >
              kMaximumPayloadSize) {
        recover_from_byte(byte);
        return std::nullopt;
      }

      expected_frame_size_ =
          sizeof(Header) +
          header_.payload_size +
          sizeof(uint16_t);

      state_ = State::kPayloadAndCrc;
      return std::nullopt;

    case State::kPayloadAndCrc:
      candidate_.push_back(byte);

      if (
          candidate_.size() <
          expected_frame_size_) {
        return std::nullopt;
      }

      if (
          candidate_.size() >
          expected_frame_size_) {
        recover_from_byte(byte);
        return std::nullopt;
      }

      if (!decode_frame(candidate_).has_value()) {
        recover_from_byte(byte);
        return std::nullopt;
      }

      {
        std::vector<uint8_t> completed =
            candidate_;

        reset();
        return completed;
      }
  }

  reset();
  return std::nullopt;
}

}  // namespace mobile_robot_hardware::protocol
