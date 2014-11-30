#include "Loaders\ImageLoader.h"
#include "gwTGA.h"
#include "common.h"

namespace gw {
	namespace loaders {

		class TGALoaderListener : public tga::ITGALoaderListener {
		public:

			TGALoaderListener() : imageBuffer(NULL), colorMapBuffer(NULL), imageBufferSize(0), colorMapBufferSize(0) { }
			~TGALoaderListener() {
				if (imageBuffer != NULL) delete[] imageBuffer;
				if (colorMapBuffer != NULL) delete[] colorMapBuffer;

				imageBuffer = NULL;
				imageBuffer = NULL;
			}

			char* operator() (const unsigned int &bitsPerPixel, const unsigned int &width, const unsigned int &height, tga::TGAMemoryType mType) {

				size_t size = (bitsPerPixel / 8) * (height * width);
				if (size == 0) return NULL;

				if (mType == tga::GWTGA_IMAGE_DATA) {
					// image data

					if (imageBufferSize < size) {
						if (imageBuffer != NULL) delete[] imageBuffer;
						imageBuffer = new char[size];
						imageBufferSize = size;
					}

					return imageBuffer;
				} else {
					// color map

					if (colorMapBufferSize < size) {
						if (colorMapBuffer != NULL) delete[] colorMapBuffer;
						colorMapBuffer = new char[size];
						colorMapBufferSize = size;
					}

					return colorMapBuffer;
				}
			}
		private:

			char* imageBuffer;
			size_t imageBufferSize;
			char* colorMapBuffer;
			size_t colorMapBufferSize;
		};

		TGALoaderListener tgaLoaderListener;

		Image loadImage(const char* fileName) {
			Image result;

			TRACE_LOADER("Loading image " << fileName << "...");

			// TODO: only tga support so far
			if (true) // if TGA
			{
				tga::TGAImage tgaImage = tga::LoadTga((char*) fileName, &tgaLoaderListener);

				if (tgaImage.hasError()) {

					// TODO: Error reporting
					TRACE_ERROR("Error while loading image." << std::cout);

					return result;
				}

				result.bytes = tgaImage.bytes;
				result.width = tgaImage.width;
				result.height = tgaImage.height;
				result.bitsPerPixel = tgaImage.bitsPerPixel;

				TRACE("OK" << std::endl);
				return result;
			}

			// be sure to load image by specialized format loader, or return error
			assert(false);
		}

	}
}