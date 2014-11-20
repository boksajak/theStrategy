#pragma once

namespace gw {
	namespace loaders {

		struct Image {

			Image() : bytes(0), width(0), height(0), bitsPerPixel(0) {}

			char* bytes;
			unsigned int width;
			unsigned int height;
			char bitsPerPixel;

			bool isError() { return bytes == 0; }
		};

		Image loadImage(const char* fileName);

	}
}