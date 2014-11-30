#include "World.h"
#include "common.h"

namespace gw {

	bool World::Initialize(Renderer &renderer) {

		if (!gui.Initialize(renderer)) {
			return false;
		}

		return true;
	}

	void World::Destroy(){
		
	}

	void World::Tick(Renderer &renderer, Input &input){
		gui.FrameStart();
		gui.Update(input);
		gui.Render(renderer);
	}
}