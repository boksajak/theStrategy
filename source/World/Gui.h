#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "World\Button.h"

namespace gw {

	class GUI {
	public:

		bool Initialize(Renderer &renderer);

		void Tick(Renderer &renderer, Input &input);

	private:
		size_t inactiveTexID;
		size_t activeTexID;
		size_t overTexID;
		
		Button okBtn;
	};
}