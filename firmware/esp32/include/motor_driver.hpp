#pragma once
#include <Arduino.h>
#include <cstdint>
namespace mobile_robot { class MotorDriver { public: MotorDriver(int pwm_pin,int direction_pin,uint8_t pwm_channel,bool inverted); void begin(uint32_t frequency_hz,uint8_t resolution_bits); void set(float normalized_output); void stop(); private: int pwm_pin_; int direction_pin_; uint8_t pwm_channel_; bool inverted_; uint16_t pwm_max_{0}; }; }
