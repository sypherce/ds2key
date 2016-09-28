#pragma once

#include <cstdint> // uint16_t
#include <string>  // std::string

namespace D2K {

extern const std::string VERSION_STRING;

// Default port
const static uint16_t DEFAULT_PORT = 9501;

// return: converts long into std::string
std::string ltos (long t);
// return: converts long into const char*
const char* ltoa (long t);
// return: converts std::string into long
long stol(std::string str);

// return: converts std::string into uint8_t
uint8_t string_to_uint8_t(std::string str);
// return: converts std::string into uint16_t
uint16_t string_to_uint16_t(std::string str);

// We'll have c++17 someday!
// TODO: comment
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return std::max(lo, std::min(v, hi ));
}

} // namespace D2K
