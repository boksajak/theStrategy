#include "common.h"
#include "Gui.h"
#include "Game\Renderer.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool GUI::Initialize() {

		// Load inactive texture
		loaders::Image img = loaders::loadImage("assets/mandrill.tga");
		
		return true;
	}

}