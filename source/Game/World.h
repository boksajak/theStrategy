#pragma once

#include "Game\Renderer.h"
#include "World\Gui.h"


namespace gw {

	class World {
	public:

		bool Initialize(Renderer &renderer);
		void Destroy();

		void Tick(Renderer &renderer);

	private:

		GUI gui;

	};
}