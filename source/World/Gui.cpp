#include "common.h"
#include "Gui.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool GUI::Initialize(Renderer &renderer) {

		// -----------------------------------------------------------
		//  Load GUI textures
		// -----------------------------------------------------------
		loaders::Image img;

		// Load inactive texture
		img = loaders::loadImage("assets/mandrill.tga");
		
		inactiveTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (inactiveTexID == -1) return false;

		// Load active texture
		img = loaders::loadImage("assets/mandrill.tga");
		
		activeTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (activeTexID == -1) return false;

		// Load over texture
		img = loaders::loadImage("assets/mandrill.tga");
		
		overTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (overTexID == -1) return false;
	
		// -----------------------------------------------------------
		//  Initialize buttons
		// -----------------------------------------------------------
		okBtn.Initialize(inactiveTexID, overTexID, activeTexID, AABB2D(glm::vec2(0.2f), glm::vec2(0.3f)));

		return true;
	}

	void GUI::Tick(Renderer &renderer) {

		// Render GUI
		renderer.RenderBillboard(inactiveTexID, glm::vec2(-1.0f, -1.0f), glm::vec2(2.0f, 2.0f), glm::vec2(0), glm::vec2(1), 0, -1);  
		//renderer.RenderBillboard(inactiveTexID, glm::vec2(0, 0), glm::vec2(0.4f, 0.4f), glm::vec2(0), glm::vec2(1), 0, -1);  

		// Render Buttons
		okBtn.Tick(renderer);
	}


}