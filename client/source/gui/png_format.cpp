#include <string>  // std::string
#ifdef _3DS
#include <png.h>
#include "common/easylogging++Wrapper.h"
#endif
#include "edit.h"
#include "gui.h"

namespace D2K {namespace GUI{

bool LoadPngImage(char *filename, int &width, int &height, unsigned char **output)
{	
#if defined(_3DS)
	// Open png file
    FILE *file = fopen(filename, "rb");
    if(file == nullptr)
	{
		LOG(ERROR) << "Issue reading " << filename << ".";
        return false;
	}
 
    // Initialize png read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(png_ptr == nullptr)
	{
		// Close file
        fclose(file);
		LOG(ERROR) << "Issue initialize png read struct from " << filename << ".";
        return false;
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
        return false;
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
			return false;
		}
	}
 
    // Setup libpng IO
    png_init_io(png_ptr, file);
 
    // Set libpng sig bytes
    unsigned int sig_read = 0;
    png_set_sig_bytes(png_ptr, sig_read);
 
    // read the entire PNG into memory
	// PNG_TRANSFORM_STRIP_16   : Strip 16-bit samples to 8 bits
	// PNG_TRANSFORM_PACKING    : Expand 1, 2 and 4-bit samples to bytes
	// PNG_TRANSFORM_EXPAND     : Expand paletted images to RGB, grayscale to 8-bit images and tRNS chunks to alpha channels
	// PNG_TRANSFORM_BGR        : Flip RGB to BGR, RGBA to BGRA
	// PNG_TRANSFORM_STRIP_ALPHA: Discard the alpha channel
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_BGR | PNG_TRANSFORM_STRIP_ALPHA, nullptr);
 
	// Read some variables, width, height, depth...
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type,
                 &interlace_type, nullptr, nullptr);

	// Allocate the output data
	uint32_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *output = (unsigned char*) malloc(row_bytes * height);

	// Copy the png file to the output
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for(int y = 0; y < height; y++)
	{
        memcpy(*output+(row_bytes * y), row_pointers[y], row_bytes);
    }
 
	// Deinitialize png read struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
 
	// Close file
    fclose(file);
 
	LOG(INFO) << "Loaded " << filename << ".";
    return true;
#elif defined(_NDS)
	return false;
#endif
}

}}//namespace D2K::GUI
