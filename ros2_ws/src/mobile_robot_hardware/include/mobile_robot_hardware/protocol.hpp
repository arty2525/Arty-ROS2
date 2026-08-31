#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <vector>
namespace mobile_robot_hardware::protocol {
constexpr uint16_t kMagic=0xA55A; constexpr uint8_t kVersion=1; constexpr std::size_t kMaximumPayloadSize=64;
enum class MessageType:uint8_t{kCommandVelocity=0x01,kCommandEnable=0x02,kCommandClearFaults=0x03,kCommandSetPid=0x04,kHeartbeat=0x05,kCommandConfigure=0x06,kTelemetry=0x81,kAck=0x82};
enum class AckStatus:uint8_t{kAccepted=0,kInvalidPayload=1,kInvalidConfiguration=2,kRejectedByState=3,kUnsupported=4};
#pragma pack(push,1)
struct Header{uint16_t magic;uint8_t version;uint8_t type;uint16_t sequence;uint16_t payload_size;}; struct VelocityPayload{float left_rad_per_sec;float right_rad_per_sec;}; struct EnablePayload{uint8_t enabled;}; struct HeartbeatPayload{uint8_t reserved;}; struct ConfigurePayload{float ticks_per_revolution;uint8_t left_motor_inverted;uint8_t right_motor_inverted;uint8_t left_encoder_inverted;uint8_t right_encoder_inverted;}; struct PidPayload{float kp;float ki;float kd;float feed_forward;}; struct TelemetryPayload{int64_t left_ticks;int64_t right_ticks;float left_velocity_rad_per_sec;float right_velocity_rad_per_sec;uint32_t fault_flags;uint8_t enabled;uint8_t configured;}; struct AckPayload{uint8_t command_type;uint8_t status;};
#pragma pack(pop)
struct Packet{Header header{};std::array<uint8_t,kMaximumPayloadSize> payload{};uint16_t crc{0};}; uint16_t crc16_ccitt_false(const uint8_t* data,std::size_t size); std::optional<Packet> decode_frame(const std::vector<uint8_t>& frame);
template<typename T> bool decode_payload(const Packet& packet,T& output){if(packet.header.payload_size!=sizeof(T))return false;static_assert(sizeof(T)<=kMaximumPayloadSize);std::memcpy(&output,packet.payload.data(),sizeof(T));return true;}
template<typename T> std::vector<uint8_t> encode_frame(MessageType type,uint16_t sequence,const T& payload){static_assert(sizeof(T)<=kMaximumPayloadSize);Header header{kMagic,kVersion,static_cast<uint8_t>(type),sequence,static_cast<uint16_t>(sizeof(T))};std::vector<uint8_t> frame(sizeof(Header)+sizeof(T)+sizeof(uint16_t));std::memcpy(frame.data(),&header,sizeof(Header));std::memcpy(frame.data()+sizeof(Header),&payload,sizeof(T));const uint16_t crc=crc16_ccitt_false(frame.data(),sizeof(Header)+sizeof(T));std::memcpy(frame.data()+sizeof(Header)+sizeof(T),&crc,sizeof(crc));return frame;}
}  // namespace mobile_robot_hardware::protocol
