#include "common.h"
#include "Map.h"
#include "Utils\Geometry.h"

namespace gw {

	bool Map::Initialize(size_t mapTexID, size_t stencilMaskID, glm::vec2 size, glm::ivec2 gridSize) {
		this->mapTexID = mapTexID;
		this->stencilMaskID = stencilMaskID;
		this->size = size;
		this->gridSize = gridSize;

		return true;
	}

	void Map::Update(Input &input) {
		
		position = input.state.mouse.position;
	}

	void Map::Render(Renderer &renderer) {
		renderer.UseStencilMask(stencilMaskID);
		renderer.RenderBillboard(mapTexID, glm::vec2(position), glm::vec2(size), glm::vec2(0), glm::vec2(size * 3.0f), 0, 20.0f);  
	}

}