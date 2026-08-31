#pragma once
#include <Arduino.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "config.hpp"
namespace mobile_robot::protocol {
enum class MessageType : uint8_t { kCommandVelocity=0x01, kCommandEnable=0x02, kCommandClearFaults=0x03, kCommandSetPid=0x04, kHeartbeat=0x05, kCommandConfigure=0x06, kTelemetry=0x81, kAck=0x82 };
enum class AckStatus : uint8_t { kAccepted=0, kInvalidPayload=1, kInvalidConfiguration=2, kRejectedByState=3, kUnsupported=4 };
#pragma pack(push,1)
struct Header { uint16_t magic; uint8_t version; uint8_t type; uint16_t sequence; uint16_t payload_size; };
struct VelocityPayload { float left_rad_per_sec; float right_rad_per_sec; };
struct EnablePayload { uint8_t enabled; };
struct HeartbeatPayload { uint8_t reserved; };
struct ConfigurePayload { float ticks_per_revolution; uint8_t left_motor_inverted; uint8_t right_motor_inverted; uint8_t left_encoder_inverted; uint8_t right_encoder_inverted; };
struct PidPayload { float kp; float ki; float kd; float feed_forward; };
struct TelemetryPayload { int64_t left_ticks; int64_t right_ticks; float left_velocity_rad_per_sec; float right_velocity_rad_per_sec; uint32_t fault_flags; uint8_t enabled; uint8_t configured; };
struct AckPayload { uint8_t command_type; uint8_t status; };
#pragma pack(pop)
struct Packet { Header header{}; uint8_t payload[config::kMaximumPayloadSize]{}; uint16_t crc{0}; };
uint16_t crc16_ccitt_false(const uint8_t* data,size_t size);
class Parser { public: bool push(uint8_t byte,Packet& packet); void reset(); private: enum class State { kHeader,kPayload,kCrc }; State state_{State::kHeader}; Packet packet_{}; size_t index_{0}; uint8_t crc_bytes_[2]{}; };
template<typename T> bool decode_payload(const Packet& packet,T& output){if(packet.header.payload_size!=sizeof(T))return false;static_assert(sizeof(T)<=config::kMaximumPayloadSize);std::memcpy(&output,packet.payload,sizeof(T));return true;}
template<typename T> size_t encode_packet(MessageType type,uint16_t sequence,const T& payload,uint8_t* output,size_t output_capacity){static_assert(sizeof(T)<=config::kMaximumPayloadSize);Header header{config::kProtocolMagic,config::kProtocolVersion,static_cast<uint8_t>(type),sequence,static_cast<uint16_t>(sizeof(T))};const size_t frame_size=sizeof(Header)+sizeof(T)+sizeof(uint16_t);if(output_capacity<frame_size)return 0;std::memcpy(output,&header,sizeof(Header));std::memcpy(output+sizeof(Header),&payload,sizeof(T));const uint16_t crc=crc16_ccitt_false(output,sizeof(Header)+sizeof(T));std::memcpy(output+sizeof(Header)+sizeof(T),&crc,sizeof(crc));return frame_size;}
}  // namespace mobile_robot::protocol
