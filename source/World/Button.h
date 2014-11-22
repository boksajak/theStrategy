#pragma once
#include "Game\Renderer.h"
#include "Utils\Geometry.h"

namespace gw {

	class Button {
	public:

		Button() : upTexID(-1), overTexID(-1), downTexID(-1), btnState(GW_BTNSTATE_UP) {}

		bool Initialize(size_t upTexID, size_t overTexID, size_t downTexID, AABB2D region);

		void Tick(Renderer &renderer);

	private:
		size_t upTexID;
		size_t overTexID;
		size_t downTexID;

		// Box representing button in screen coordinates
		AABB2D region;

		enum {
			GW_BTNSTATE_UP,
			GW_BTNSTATE_DOWN,
			GW_BTNSTATE_OVER
		} btnState;
	};
}