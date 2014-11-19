#include "Renderer.h"
#include "Utils\Geometry.h"


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

		glClearColor(1.0f, 1.0f, 0.6f, 1.0f);

		TRACE_RENDERER("Initialized OpenGL!" << std::endl);
		return true;
	}

	void Renderer::Destroy(){
		SDL_GL_DeleteContext(sdlGLContext);
		SDL_DestroyWindow(sdlWindow);
		SDL_Quit();
	}

	void Renderer::Render(const World &world) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Maybe insertion sort (or shell sort) will be faster than priority queue for our purpose
		// Render objects front to back
		while (!renderObjects.empty()) {
			GLRenderObject renderObject = renderObjects.top();

			// TODO: Render

			renderObjects.pop();
		}

		// Swap buffers
		SDL_GL_SwapWindow(sdlWindow);
	}

	void Renderer::RenderTexture(size_t texIdx, glm::vec2 screenCoords, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z) {

		// Culling
		static const glm::vec4 screenBox(-1.0f, -1.0f, 1.0f, 1.0f);
		if (!BoxIntersect(glm::vec4(screenCoords.x, screenCoords.y, screenCoords.x + size.x, screenCoords.y + size.y), screenBox)) return;

		// Put object into queue
		renderObjects.push(GLRenderObject(textures[texIdx], screenCoords, size, uvTopLeft, uvBottomRight, rotation, z));
	}

	size_t Renderer::UploadTexture(char* bytes, size_t width, size_t height, char bitsPerPixel) {

		GLuint textureId;

		GLenum format;
		GLenum type;

		switch(bitsPerPixel) {
		case 8:
			format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
			break;
		case 16:
			format = GL_BGRA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			break;
		case 24:
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			break;
		case 32:
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			break;
		case 96:
			format = GL_BGR;
			type = GL_FLOAT;
			break;
		default:
			TRACE_ERROR("Not supported pixel depth of " << (int)bitsPerPixel << "bpp encountered" << std::endl);
			return -1;
		}

		// Load Texture to GPU
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, bytes);

		glGenerateMipmap(GL_TEXTURE_2D);   

		// Trilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Enable anisotropic filtering if supported 
		if(glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
			GLfloat fLargest;

			// Get largest anisotropy level supported
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, fLargest)); // we want level 4 if possible
		}
		
		textures.push_back(GLTexture(textureId));

		return textures.size() - 1;
	}
}