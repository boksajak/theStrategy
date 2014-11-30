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
		img = loaders::loadImage("assets/mandrill_bw.tga");
		
		inactiveTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (inactiveTexID == -1) return false;

		// Load active texture
		img = loaders::loadImage("assets/mandrill_cr.tga");
		
		activeTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (activeTexID == -1) return false;

		// Load over texture
		img = loaders::loadImage("assets/mandrill.tga");
		
		overTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (overTexID == -1) return false;
	
		// -----------------------------------------------------------
		//  Initialize buttons
		// -----------------------------------------------------------
		okBtn.Initialize(inactiveTexID, overTexID, activeTexID, AABB2D(glm::vec2(0.1f), glm::vec2(0.4f)));

		return true;
	}

	void GUI::Update(Input &input) {

		// Render Buttons
		okBtn.Update(input);
	}

	void GUI::Render(Renderer &renderer) {

		// Render GUI
		renderer.RenderBillboard(inactiveTexID, glm::vec2(-1.0f, -1.0f), glm::vec2(2.0f, 2.0f), glm::vec2(0), glm::vec2(1), 0, -0.5f);  

		// Render Buttons
		okBtn.Render(renderer);
	}

	void GUI::FrameStart() {
		Button::FrameStart();
	}

}