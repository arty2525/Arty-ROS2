#include "encoder_reader.hpp"
namespace mobile_robot { EncoderReader::EncoderReader(const int pin_a,const int pin_b,const bool inverted):encoder_(pin_a,pin_b),inverted_(inverted){} int64_t EncoderReader::ticks() const{const int64_t value=static_cast<int64_t>(encoder_.read());return inverted_?-value:value;} void EncoderReader::reset(){encoder_.write(0);} }
