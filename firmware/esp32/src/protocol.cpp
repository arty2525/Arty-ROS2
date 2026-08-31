#include "protocol.hpp"

namespace mobile_robot::protocol {

namespace {

constexpr uint8_t kMagicLow =
    static_cast<uint8_t>(
        config::kProtocolMagic & 0x00FFU);

constexpr uint8_t kMagicHigh =
    static_cast<uint8_t>(
        (config::kProtocolMagic >> 8U) &
        0x00FFU);

}  // namespace

uint16_t crc16_ccitt_false(
    const uint8_t* data,
    const size_t size) {
  uint16_t crc = 0xFFFFU;

  for (size_t index = 0U;
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

void Parser::begin_candidate() {
  packet_ = Packet{};
  crc_bytes_[0] = 0U;
  crc_bytes_[1] = 0U;

  auto* header_bytes =
      reinterpret_cast<uint8_t*>(
          &packet_.header);

  header_bytes[0] = kMagicLow;
  index_ = 1U;
  state_ = State::kMagicHigh;
}

void Parser::recover_from_byte(
    const uint8_t byte) {
  reset();

  if (byte == kMagicLow) {
    begin_candidate();
  }
}

void Parser::reset() {
  state_ = State::kMagicLow;
  packet_ = Packet{};
  index_ = 0U;
  crc_bytes_[0] = 0U;
  crc_bytes_[1] = 0U;
}

bool Parser::push(
    const uint8_t byte,
    Packet& packet) {
  switch (state_) {
    case State::kMagicLow:
      if (byte == kMagicLow) {
        begin_candidate();
      }
      return false;

    case State::kMagicHigh: {
      if (byte == kMagicHigh) {
        auto* header_bytes =
            reinterpret_cast<uint8_t*>(
                &packet_.header);

        header_bytes[1] = byte;
        index_ = 2U;
        state_ = State::kHeaderRest;
        return false;
      }

      if (byte == kMagicLow) {
        begin_candidate();
        return false;
      }

      reset();
      return false;
    }

    case State::kHeaderRest: {
      auto* header_bytes =
          reinterpret_cast<uint8_t*>(
              &packet_.header);

      header_bytes[index_++] = byte;

      if (index_ < sizeof(Header)) {
        return false;
      }

      if (
          packet_.header.magic !=
              config::kProtocolMagic ||
          packet_.header.version !=
              config::kProtocolVersion ||
          packet_.header.payload_size >
              config::kMaximumPayloadSize) {
        recover_from_byte(byte);
        return false;
      }

      index_ = 0U;

      state_ =
          packet_.header.payload_size == 0U
              ? State::kCrc
              : State::kPayload;

      return false;
    }

    case State::kPayload:
      packet_.payload[index_++] = byte;

      if (
          index_ ==
          packet_.header.payload_size) {
        index_ = 0U;
        state_ = State::kCrc;
      }

      return false;

    case State::kCrc:
      crc_bytes_[index_++] = byte;

      if (index_ < sizeof(uint16_t)) {
        return false;
      }

      uint16_t received_crc = 0U;

      std::memcpy(
          &received_crc,
          crc_bytes_,
          sizeof(received_crc));

      uint8_t buffer[
          sizeof(Header) +
          config::kMaximumPayloadSize]{};

      std::memcpy(
          buffer,
          &packet_.header,
          sizeof(Header));

      std::memcpy(
          buffer + sizeof(Header),
          packet_.payload,
          packet_.header.payload_size);

      const uint16_t calculated_crc =
          crc16_ccitt_false(
              buffer,
              sizeof(Header) +
                  packet_.header.payload_size);

      if (received_crc == calculated_crc) {
        packet_.crc = received_crc;
        packet = packet_;
        reset();
        return true;
      }

      recover_from_byte(byte);
      return false;
  }

  reset();
  return false;
}

}  // namespace mobile_robot::protocol
