#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "World\Button.h"

namespace gw {

	class GUI {
	public:

		bool Initialize(Renderer &renderer);

		void Update(Input &input);
		void Render(Renderer &renderer);

		static void FrameStart();

	private:
		size_t inactiveTexID;
		size_t activeTexID;
		size_t overTexID;
		
		Button okBtn;
	};
}