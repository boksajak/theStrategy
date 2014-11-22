#pragma once
#include "common.h"
#include "glm.h"

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
	
	struct GLBillboard {

		GLBillboard(GLTexture texture, glm::vec2 topLeftCoord, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z) : texture(texture),
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

	// Comparer for Billboards
	inline bool operator<(const GLBillboard& a, const GLBillboard& b){ return a.z < b.z; }

	struct BillboardShader {

		BillboardShader() : programId(-1) {}

		~BillboardShader() {
			if (programId != -1) {
				glDeleteProgram(programId);
			}
		}

		GLuint programId;

		GLuint screenCoordsLoc;
		GLuint sizeLoc;
		GLuint topLeftUVLoc;
		GLuint bottomRightUVLoc;

		GLuint textureLoc;
	};

	class Renderer {
	public:

		Renderer() { }

		bool Initialize(int windowWidth, int windowHeight);
		void Destroy();

		void Render();
		void RenderBillboard(size_t texIdx, glm::vec2 screenCoords, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z);

		size_t UploadTexture(char* bytes, size_t width, size_t height, char bitsPerPixel);

	private:

		bool InitializeBillboards();
		bool LoadShader(const char* fileName, GLuint &programId);

		void printShaderInfoLog(GLuint obj);
		void printProgramInfoLog(GLuint obj);

		// Renderer stuff
		std::vector<GLTexture> textures;
		std::priority_queue<GLBillboard> billboards;

		// SDL Stuff
		SDL_Window* sdlWindow;
		SDL_GLContext sdlGLContext;

		// OpenGL stuff
		GLuint vaoBillboard;
		BillboardShader billboardShader;

	};
}
