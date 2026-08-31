#pragma once
#include <Encoder.h>
#include <cstdint>
namespace mobile_robot { class EncoderReader { public: EncoderReader(int pin_a,int pin_b,bool inverted); int64_t ticks() const; void reset(); private: mutable Encoder encoder_; bool inverted_; }; }
