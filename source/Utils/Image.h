#pragma once

namespace gw {

	struct Image {

			Image() : bytes(0), width(0), height(0), bitsPerPixel(0) {}

			char* bytes;
			unsigned int width;
			unsigned int height;
			char bitsPerPixel;

			bool isError() { return bytes == 0; }
		};
}