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
		GLTexture(GLuint textureId, bool hasAlpha) : textureId(textureId), hasAlpha(hasAlpha) {}

		GLuint textureId;
		bool hasAlpha;
	};
	
	struct GLBillboard {

		GLBillboard(GLTexture texture, glm::vec2 topLeftCoord, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z, size_t stencilMaskId) : texture(texture),
			topLeftCoord(topLeftCoord), size(size), uvTopLeft(uvTopLeft), uvBottomRight(uvBottomRight), rotation(rotation), z(z), stencilMaskId(stencilMaskId) {}

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

		// stencil mask to use
		size_t stencilMaskId;

	};

	struct GLMask {
		GLMask(GLuint textureId) : textureId(textureId) {}
		GLuint textureId;
	};

	// Comparer for Billboards (opaque first - back to front, transparent last - back to front) 
	inline bool operator<(const GLBillboard& a, const GLBillboard& b){ 

		if (a.texture.hasAlpha == b.texture.hasAlpha) {

			// TODO: group same stencil masks together

			if (a.texture.hasAlpha) {
				return a.z < b.z; // Render transparent back to front
			} else {
				return a.z > b.z; // Render opaque front to back
			}
		}

		// Put transparent after opaque
		return a.texture.hasAlpha;
	}

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

		GLuint stencilPassLoc;
		GLuint normalPassLoc;
	};

	class Renderer {
	public:

		Renderer() { }

		bool Initialize(int windowWidth, int windowHeight);
		void Destroy();

		void Render();
		void RenderBillboard(size_t texIdx, glm::vec2 screenCoords, glm::vec2 size, glm::vec2 uvTopLeft, glm::vec2 uvBottomRight, float rotation, float z);

		void UseStencilMask(size_t maskId);

		size_t UploadTexture(char* bytes, size_t width, size_t height, char bitsPerPixel);
		size_t UploadStencilMask(size_t maskTexId);

		glm::vec2 screenSize;

	private:

		bool InitializeBillboards();
		bool LoadShader(const char* fileName, GLuint &programId);

		void printShaderInfoLog(GLuint obj);
		void printProgramInfoLog(GLuint obj);

		// Renderer stuff
		std::vector<GLTexture> textures;
		std::vector<GLMask> masks;
		std::priority_queue<GLBillboard> billboards;
		size_t activeStencilMaskId;

		// SDL Stuff
		SDL_Window* sdlWindow;
		SDL_GLContext sdlGLContext;

		// OpenGL stuff
		GLuint vaoBillboard;
		BillboardShader billboardShader;

	};
}
