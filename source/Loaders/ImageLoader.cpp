#include "Loaders\ImageLoader.h"
#include "gwTGA.h"
#include "common.h"

namespace gw {
	namespace loaders {

		Image loadImage(const char* fileName) {
			Image result;

			TRACE_LOADER("Loading image " << fileName << "..." << std::endl);

			// TODO: only tga support so far
			if (true) // if TGA
			{
				tga::TGAImage tgaImage = tga::LoadTgaFromFile((char*) fileName);

				if (tgaImage.error != tga::GWTGA_NONE) {

					// TODO: Error reporting
					TRACE_ERROR("Error while loading image." << std::cout);

					return result;
				}

				result.bytes = tgaImage.bytes;
				result.width = tgaImage.width;
				result.height = tgaImage.height;
				result.bitsPerPixel = tgaImage.bitsPerPixel;

				return result;
			}

			// be sure to load image by specialized format loader, or return error
			assert(false);
		}
	}
}