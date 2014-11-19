#pragma once

#include "World\Gui.h"


namespace gw {

	class World {
	public:

		bool Initialize();
		void Destroy();

	private:

		GUI gui;

	};
}