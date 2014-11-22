#pragma once
#include "Game\Renderer.h"
#include "World\Button.h"

namespace gw {

	class GUI {
	public:

		bool Initialize(Renderer &renderer);

		void Tick(Renderer &renderer);

	private:
		size_t inactiveTexID;
		size_t activeTexID;
		size_t overTexID;
		
		Button okBtn;
	};
}