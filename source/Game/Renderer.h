#pragma once

#include "World.h"

// Include SDL
#include "SDL.h"

namespace gw {

	class Renderer {
	public:

		bool Initialize(int windowWidth, int windowHeight);
		void Destroy();

		void Render(const World &world);

	private:

		SDL_Window* sdlWindow;
		SDL_GLContext sdlGLContext;

	};
}
