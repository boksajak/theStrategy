#pragma once
#include "Utils\Image.h"

namespace gw {
	namespace loaders {

		Image loadImage(const char* fileName);
		Image loadImage(const char* fileName, bool useBuffer);

	}
}