#include "common.h"
#include "Gui.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool GUI::Initialize(Renderer &renderer) {

		// -----------------------------------------------------------
		//  Load GUI textures
		// -----------------------------------------------------------
		Image img;

		// Load inactive texture
		img = loaders::loadImage("assets/gui_bw.tga", true);
		
		inactiveTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (inactiveTexID == -1) return false;

		// Load active texture
		img = loaders::loadImage("assets/gui_over.tga", true);
		
		activeTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (activeTexID == -1) return false;

		// Load over texture
		img = loaders::loadImage("assets/gui_down.tga", true);
		
		overTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (overTexID == -1) return false;
	
		// Load GUI stencil
		img = loaders::loadImage("assets/gui_stencil.tga", true);
		
		stencilTexID = renderer.UploadTexture(img.bytes, img.width, img.height, img.bitsPerPixel);
		if (stencilTexID == -1) return false;

		// -----------------------------------------------------------
		//  Initialize buttons
		// -----------------------------------------------------------
		okAction = OKButtonAction("OK Clicked!");
		if (!okBtn.Initialize(inactiveTexID, overTexID, activeTexID, AABB2D(glm::vec2(0.730914f, 0.69783f), glm::vec2(0.801001f, 0.767947f)), &okAction)) {
			return false;
		}

		// -----------------------------------------------------------
		//  Initialize cursors
		// -----------------------------------------------------------
		img = loaders::loadImage("assets/cursor_mouse.tga", false);
		Image imgDown = loaders::loadImage("assets/cursor_mouse_down.tga", false);
	
		if (!arrowCursor.Initialize(img, imgDown, glm::vec2(img.width, img.height), renderer.screenSize)) {
			return false;
		}

		return true;
	}

	void GUI::Update(Input &input) {

		// Print mouse position
		if (input.state.mouse.leftClick)
			TRACE_WARNING("X: " << input.state.mouse.position.x << " Y: " << input.state.mouse.position.y << std::endl);

		// Update Buttons
		okBtn.Update(input);

		// Update Cursor
		arrowCursor.Update(input);
	}

	void GUI::Render(Renderer &renderer) {

		// Render GUI (background)
		renderer.RenderBillboard(inactiveTexID, glm::vec2(0.0f), glm::vec2(1.f), glm::vec2(0), glm::vec2(1), 0, 100.0f);  

		// Render Buttons
		okBtn.Render(renderer);

		// Render Cursor
		arrowCursor.Render(renderer);
	}

	void GUI::FrameStart() {
		Button::FrameStart();
	}

}