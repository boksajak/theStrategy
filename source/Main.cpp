#include "common.h"

// Include GLEW
#include "GL/glew.h"
#include "GL/wglew.h"

// Include SDL
#include "SDL.h"

SDL_Window* sdlWindow;
SDL_GLContext sdlGLContext;

// http://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution
// Get the horizontal and vertical screen sizes in pixel (of primary monitor)
void GetDesktopResolution(int& screenWidth, int& screenHeight, int& workAreaWidth, int& workAreaHeight)
{
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	workAreaWidth = rect.right;
	workAreaHeight = rect.bottom;
}

bool Initialize() {

	// -------------------------------------------
	// Initialize SDL 
	// -------------------------------------------
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TRACE_ERROR("Error while initializing SDL." << std::endl);
		return false;
	}

	// Double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// 24-bit Z buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window 
	sdlWindow = SDL_CreateWindow("The Strategy Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL); 

	if (!sdlWindow) {
		TRACE_ERROR("Error while creating SDL window." << std::endl);
		TRACE_ERROR(SDL_GetError() << std::endl);
		return false;
	}

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

void Render() {
	glClearColor(1.0f, 1.0f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(int argc, char *argv[]){

	// Initialization
	if (!Initialize()) {
		return 1;
	}

	SDL_Event event;

	bool running = true;

	// Main Loop
	while(running){

		// Get messages
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT){
				running = false;
				break;
			}
		}

		// Render
		Render();
		SDL_GL_SwapWindow(sdlWindow);
	} 

	// Cleanup
	SDL_GL_DeleteContext(sdlGLContext);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();

	return 0;
}

