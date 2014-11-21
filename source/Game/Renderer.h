#pragma once
#include "common.h"
#include "glm.h"

#include "World.h"

// Include GLEW
#include "GL/glew.h"
#include "GL/wglew.h"

// Include SDL
#include "SDL.h"

namespace gw {

	struct GLTexture {
		GLTexture(GLuint textureId) : textureId(textureId) {}

		GLuint textureId;
	};

	struct GLRenderObject {

		GLRenderObject(GLTexture texture, glm::vec2 topLeftCoord, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z) : texture(texture),
		topLeftCoord(topLeftCoord), size(size), uvTopLeft(uvTopLeft), uvBottomRight(uvBottomRight), rotation(rotation), z(z) {}

		// texture to use
		GLTexture texture;

		// position of top left corner in screen space		
		glm::vec2 topLeftCoord;

		// size in screen space
		glm::vec2 size;
		
		// uv coordinates of top left corner
		glm::vec2 uvTopLeft;
		
		// uv coordinates of bottom right corner
		glm::vec2 uvBottomRight;
		
		// rotation in radians (clockwise)
		float rotation;

		// depth (less is closer to viewer) 
		float z;
	};

	// Comparer for Render objects
	inline bool operator<(const GLRenderObject& a, const GLRenderObject& b){ return a.z < b.z; }


	class Renderer {
	public:

		Renderer() : billboardProgramId(-1) { }

		bool Initialize(int windowWidth, int windowHeight);
		void Destroy();

		void Render(const World &world);

		void RenderBillboard(size_t texIdx, glm::vec2 screenCoords, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z);
		size_t UploadTexture(char* bytes, size_t width, size_t height, char bitsPerPixel);

	private:

		bool InitializeBillboards();
		bool LoadShader(const char* fileName, GLuint &programId);

		void printShaderInfoLog(GLuint obj);
		void printProgramInfoLog(GLuint obj);

		// Renderer stuff
		std::vector<GLTexture> textures;
		std::priority_queue<GLRenderObject> renderObjects;

		// SDL Stuff
		SDL_Window* sdlWindow;
		SDL_GLContext sdlGLContext;

		// OpenGL stuff
		GLuint billboardProgramId;
		GLuint vaoBillboard;

	};
}
