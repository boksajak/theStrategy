#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "Utils\Geometry.h"

namespace gw {

	struct ButtonSharedState {
		ButtonSharedState() : hitFound(false) {}

		bool	hitFound;
	};

	class Button {
	public:

		Button() : upTexID(-1), overTexID(-1), downTexID(-1), btnState(GW_BTNSTATE_UP) {}

		bool Initialize(size_t upTexID, size_t overTexID, size_t downTexID, AABB2D region);

		void Update(Input &input);
		void Render(Renderer &renderer);

		static void FrameStart();

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

		static ButtonSharedState sharedState;
	};
}