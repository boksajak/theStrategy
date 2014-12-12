#include "common.h"
#include "Map.h"
#include "Utils\Geometry.h"

namespace gw {

	bool Map::Initialize(size_t mapTexID, size_t stencilMaskID, glm::vec2 size, glm::ivec2 gridSize, AABB2D window) {
		this->mapTexID = mapTexID;
		this->stencilMaskID = stencilMaskID;
		this->size = size;
		this->gridSize = gridSize;
		this->window = window;

		return true;
	}

	void Map::Update(Input &input) {

		if (input.state.mouse.rightClick) {
			mouseClickPosition = input.state.mouse.position;;
		}

		if (input.state.mouse.rightDown) {
			glm::vec2 motionVector = input.state.mouse.position - mouseClickPosition;
			position -= motionVector * 0.2f;

			// Stick to windows boundaries
			position = glm::min(window.minPoint, position);
			position = glm::max(window.maxPoint - size, position);

		}
	}

	void Map::Render(Renderer &renderer) {
		renderer.UseStencilMask(stencilMaskID);
		renderer.RenderBillboard(mapTexID, glm::vec2(position), glm::vec2(size), glm::vec2(0), glm::vec2(size * 2.0f), 0, 20.0f);  
	}

}