#pragma once

#include <string>

namespace D2K {namespace GUI {

// Loads and converts PNG image into a raw 24bit image
//
//  param: filename - The filename contaning a PNG image.
//  param: width    - A pointer filled in with the Width of loaded image. Left alone if image fails to load.
//  param: height   - A pointer filled in with the Height of loaded image. Left alone if image fails to load.
//  param: alpha    - A flag to enable or disable alpha transparency. False = strip transparency.
//  param: output   - A pointer filled in with the raw 24bit image data. Left alone if image fails to load.
// 
// return: Total buffer size in bytes, 0 on failure
uint32_t LoadPngImage(std::string filename, int &width, int &height, bool alpha, unsigned char **output);

}} // namespace D2K::GUI
