#include "common.h"
#include "Font.h"
#include "Loaders\ImageLoader.h"

namespace gw {
	bool Font::Initialize(char* fontFilePath, char* descriptionFilePath) {
		
		// Load font image
		fontImage = loaders::loadImage(fontFilePath, false);

		if (fontImage.isError()) {
			TRACE_ERROR("Could not initialize font. Cannot read file " << fontFilePath << std::endl);	
			return false;
		}

		// Load font description

		return true;
	}

	Image Font::GetText(char* text, float size){
		Image result;

		return result;
	}
}