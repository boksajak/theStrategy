#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "Utils\Geometry.h"
#include "Utils\Image.h"

namespace gw {

	enum SystemCursorState {
		GW_CRSRSTATE_UP,
		GW_CRSRSTATE_DOWN
	};

	class SystemCursor;

	struct SystemCursorSharedState {
		SystemCursorSharedState() : state(GW_CRSRSTATE_UP), currentCursor(NULL) {}

		SystemCursor* currentCursor;

		SystemCursorState state;
	};

	class SystemCursor {
	public:

		~SystemCursor();
		SystemCursor() : cursorState(GW_CRSRSTATE_UP), upCursor(NULL), downCursor(NULL), upSurface(NULL), downSurface(NULL) {}

		bool Initialize(const Image &upTexture, const Image &downTexture, glm::vec2 size, glm::vec2 screenSize);

		void Update(Input &input);
		void Render(Renderer &renderer);

		static void FrameStart();

	private:

		bool LoadSDLCursor(const Image& image, SDL_Surface* &surface, SDL_Cursor* &cursor);

		glm::vec2 size;
		glm::vec2 position;

		SystemCursorState cursorState;

		SDL_Surface* upSurface;
		SDL_Surface* downSurface;
		SDL_Cursor* upCursor;
		SDL_Cursor* downCursor;

		Image upImg;
		Image downImg;

		static SystemCursorSharedState sharedCursorState;

	};
}