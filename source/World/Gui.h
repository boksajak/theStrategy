#pragma once
#include "Game\Renderer.h"

namespace gw {

	class GUI {
	public:

		bool Initialize(Renderer &renderer);

		void Render(Renderer &renderer);

	private:
		size_t inactiveTexID;
	};
}