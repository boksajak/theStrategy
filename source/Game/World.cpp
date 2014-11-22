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

	void World::Tick(Renderer &renderer){
		gui.Tick(renderer);
	}
}