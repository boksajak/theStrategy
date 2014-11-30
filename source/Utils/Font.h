#pragma once
#include "Image.h"

namespace gw {

	class Font {
	public:

		bool Initialize(char* fontFilePath, char* descriptionFilePath);
		Image GetText(char* text, float size);

	private:
		Image fontImage;

		float charWidths[64];
	};
}