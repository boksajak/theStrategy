#include "World.h"
#include "common.h"

namespace gw {

	bool World::Initialize() {

		if (!gui.Initialize()) {
			return false;
		}

		return true;
	}

	void World::Destroy(){

	}
}