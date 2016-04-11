#pragma once

#include <cstdint> // uint16_t
#include <string>  // std::string

namespace D2K {

extern const std::string VERSION_STRING;

// Default port
const static uint16_t DEFAULT_PORT = 9501;

// @return converts long into std::string
extern std::string ltos (long t);
// @return converts long into const char*
extern const char* ltoa (long t);
// @return converts std::string into long
extern long stol(std::string str);

// @return converts std::string into uint8_t
extern uint8_t string_to_uint8_t(std::string str);
// @return converts std::string into uint16_t
extern uint16_t string_to_uint16_t(std::string str);

}//namespace D2K
