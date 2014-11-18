#include "Renderer.h"
#include "common.h"

// Include GLEW
#include "GL/glew.h"
#include "GL/wglew.h"

namespace gw {

	bool Renderer::Initialize(int windowWidth, int windowHeight) {

		// -------------------------------------------
		// Initialize SDL 
		// -------------------------------------------
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			TRACE_ERROR("Error while initializing SDL." << std::endl);
			return false;
		}

		// Double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

		// Assert HW Accelerated graphics
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// 24-bit Z buffer
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Get screen resolution
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
			TRACE_ERROR("SDL_GetDesktopDisplayMode failed: " << SDL_GetError() << std::endl);
			return false;
		}

		int screenWidth = displayMode.w;
		int screenHeight = displayMode.h;

		// Calculate optimal window position on screen
		int left = (screenWidth - windowWidth - 550) / 3; // 550 - make room for debugging console window
		int top = (screenHeight - windowHeight) / 3;

		// Create the window 
		sdlWindow = SDL_CreateWindow("The Strategy Game", left, top, windowWidth, windowHeight, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL); 

		if (!sdlWindow) {
			TRACE_ERROR("Error while creating SDL window." << std::endl);
			TRACE_ERROR(SDL_GetError() << std::endl);
			return false;
		}

		// Move console window to nice position right of main window
#ifdef USE_CONSOLE
		int borderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		int borderHeight = GetSystemMetrics(SM_CYSIZEFRAME);
		int captionHeight = GetSystemMetrics(SM_CYCAPTION);
		consoleInst.move(left + windowWidth + borderWidth, top - borderHeight - captionHeight, 550, windowHeight);
#endif


		// -------------------------------------------
		// Initialize OpenGL 
		// -------------------------------------------
		sdlGLContext = SDL_GL_CreateContext(sdlWindow); 

		if (!sdlGLContext) {
			TRACE_ERROR("Error while creating SDL GLContext." << std::endl);
			TRACE_ERROR(SDL_GetError() << std::endl);
			return false;
		}

		GLenum glewStatus = glewInit();

		if (GLEW_OK != glewStatus){
			TRACE_ERROR("Error: " << glewGetErrorString(glewStatus) << std::endl);
			return false;
		} 

		// Vertical Sync
		SDL_GL_SetSwapInterval(1); 

		TRACE_RENDERER("Context created: " << glGetString(GL_VERSION) << std::endl);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);   

		TRACE_RENDERER("Initialized OpenGL!" << std::endl);
		return true;
	}

	void Renderer::Destroy(){
		SDL_GL_DeleteContext(sdlGLContext);
		SDL_DestroyWindow(sdlWindow);
		SDL_Quit();
	}

	void Renderer::Render(const World &world) {
		glClearColor(1.0f, 1.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Swap buffers
		SDL_GL_SwapWindow(sdlWindow);
	}
}