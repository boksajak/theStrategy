#include "common.h"
#include "Gui.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool GUI::Initialize(Renderer &renderer) {

		// Load inactive texture
		loaders::Image img = loaders::loadImage("assets/mandrill.tga");
		
		inactiveTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (inactiveTexID == -1) return false;

		return true;
	}

	void GUI::Render(Renderer &renderer) {
		renderer.RenderBillboard(inactiveTexID, glm::vec2(0, 0), glm::vec2(0.4f, 0.4f), glm::vec2(0), glm::vec2(1), 0, -1);  
	}


}