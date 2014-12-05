#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "Utils\Geometry.h"

namespace gw {

	class Cursor {
	public:

		Cursor() : upTexID(-1), downTexID(-1), cursorState(GW_CRSRSTATE_UP) {}

		bool Initialize(size_t upTexID, size_t downTexID, glm::vec2 size, glm::vec2 screenSize);

		void Update(Input &input);
		void Render(Renderer &renderer);

		static void FrameStart();

	private:
		size_t upTexID;
		size_t downTexID;

		glm::vec2 size;
		glm::vec2 position;

		enum {
			GW_CRSRSTATE_UP,
			GW_CRSRSTATE_DOWN
		} cursorState;

	};
}