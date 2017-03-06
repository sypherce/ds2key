#include <string>  // std::string
#include <cstdlib> // malloc
#include <cstring> // memcpy
#include <iomanip> // setprecision
#include <png.h>
#include "common/easylogging++Wrapper.h"

namespace D2K {namespace GUI{

// TODO: merge GUI::LoadPngImage, GUI::LoadButtonImage, GUI::DrawButtonImage, GUI::LoadBackgroundImage, GUI::DrawBackgroundImage
// TODO: into a class
uint32_t LoadPngImage(const std::string filename, int &width, int &height, bool alpha, unsigned char **output)
{
	// Open png file
	FILE *file = fopen(filename.c_str(), "rb");
	if(file == nullptr)
	{
		LOG(ERROR) << "Issue reading " << filename << ".";
		return 0;
	}

	// Store png file size
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	LOG(INFO) << "Opening " << filename << " (" << std::fixed << std::setprecision(2) << file_size / 1000.0f / 1000.0f << "MB).";

	// Initialize png read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(png_ptr == nullptr)
	{
		// Close file
		fclose(file);
		LOG(ERROR) << "Issue initialize png read struct from " << filename << ".";
		return 0;
	}

	// Initialize png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == nullptr)
	{
		// Deinitialize png read struct
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		// Close file
		fclose(file);

		LOG(ERROR) << "Issue initializing png info struct from " << filename << ".";
		return 0;
	}

	// Setup libpng error handling
	{
		if(setjmp(png_jmpbuf(png_ptr)))
		{
			// Deinitialize png read struct
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			// Close file
			fclose(file);

			LOG(ERROR) << "Issue reading " << filename << ".";
			return 0;
		}
	}

	// Setup libpng IO
	png_init_io(png_ptr, file);

	// Set libpng sig bytes
	unsigned int sig_read = 0;
	png_set_sig_bytes(png_ptr, sig_read);

	// Read the entire PNG into memory
	// PNG_TRANSFORM_STRIP_16   : Strip 16-bit samples to 8 bits
	// PNG_TRANSFORM_PACKING    : Expand 1, 2 and 4-bit samples to bytes
	// PNG_TRANSFORM_EXPAND     : Expand paletted images to RGB, grayscale to 8-bit images and tRNS chunks to alpha channels
	// PNG_TRANSFORM_STRIP_ALPHA: Discard the alpha channel
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | (alpha ? 0 : PNG_TRANSFORM_STRIP_ALPHA), nullptr);

	// Read some variables, width, height, depth...
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type,
	             &interlace_type, nullptr, nullptr);

	// Allocate the output data
	uint32_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	uint32_t buffer_size = row_bytes * height;
	*output = (unsigned char*)malloc(buffer_size);

	// Copy the png file to the output
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for(int y = 0; y < height; y++)
	{
		memcpy(*output + (row_bytes * y), row_pointers[y], row_bytes);
	}

	// Deinitialize png read struct
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

	// Close file
	fclose(file);

	LOG(INFO) << "Loaded " << filename << ".";
	return buffer_size;
}

}} // namespace D2K::GUI
