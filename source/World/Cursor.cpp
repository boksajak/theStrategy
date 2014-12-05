#include "common.h"
#include "Cursor.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool Cursor::Initialize(size_t upTexID, size_t downTexID, glm::vec2 size, glm::vec2 screenSize) {
		this->upTexID = upTexID;
		this->downTexID = downTexID;
		this->size = absoulteToNormalized(size, screenSize);
		return true;
	}

	void Cursor::Update(Input &input) {

		if (input.state.mouse.leftDown) {
			cursorState = GW_CRSRSTATE_DOWN;
		} else {
			cursorState = GW_CRSRSTATE_UP;
		}

		position = input.state.mouse.position;
	}

	void Cursor::Render(Renderer &renderer) {

		switch (cursorState) {
		case GW_CRSRSTATE_UP:
			// TODO: Z depth
			renderer.RenderBillboard(downTexID, position, size, glm::vec2(0), glm::vec2(1), 0, 10.0f);  
			break;
		case GW_CRSRSTATE_DOWN:
			// TODO: Z depth
			renderer.RenderBillboard(downTexID, position, size, glm::vec2(0), glm::vec2(1), 0, 10.0f);  
			break;
		default:
			assert(false); // Something went very wrong
		}
	}

}