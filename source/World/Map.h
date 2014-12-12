#pragma once
#include "Game\Renderer.h"
#include "Game\Input.h"
#include "Utils\Geometry.h"

namespace gw {

	class Map {
	public:

		Map() : mapTexID(-1), stencilMaskID(-1), size(glm::vec2(0)), gridSize(glm::ivec2(0)), mouseClickPosition(glm::vec2(0)), position(glm::vec2(0)) {}

		bool Initialize(size_t mapTexID, size_t stencilMaskID, glm::vec2 size, glm::ivec2 gridSize, AABB2D window);

		void Update(Input &input);
		void Render(Renderer &renderer);

	private:
		size_t mapTexID;
		size_t stencilMaskID;
		glm::vec2 size;
		AABB2D window;
		glm::ivec2 gridSize;
		
		glm::vec2 position;
		glm::vec2 mouseClickPosition;
	};
}