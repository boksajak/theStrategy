#include "common.h"
#include "Button.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	ButtonSharedState Button::sharedState;

	bool Button::Initialize(size_t upTexID, size_t overTexID, size_t downTexID, AABB2D region) {
		this->upTexID = upTexID;
		this->overTexID = overTexID;
		this->downTexID = downTexID;

		this->region = region;
		return true;
	}

	void Button::Update(Input &input) {
		if (sharedState.hitFound) return;

		// TODO: solve different coordinates systems for textures, screen and mouse problem
		float x = input.state.mouse.position.x * 2.0f - 1.0f;
		float y = -(input.state.mouse.position.y * 2.0f - 1.0f);

		if (BoxIntersect(glm::vec2(x, y), region)) {
			sharedState.hitFound = true;

			if (input.state.mouse.leftDown) {
				btnState = GW_BTNSTATE_DOWN;
			} else {
				btnState = GW_BTNSTATE_OVER;
			}

			if (input.state.mouse.leftDown) {
				// TODO: Fire button event
			}

		} else {
			btnState = GW_BTNSTATE_UP;
		}
	}

	void Button::Render(Renderer &renderer) {

		switch (btnState) {
		case GW_BTNSTATE_UP:
			// Do nothing - assume that button is already rendered as "background" in inactive state
			break;
		case GW_BTNSTATE_DOWN:
			// TODO: Z depth
			renderer.RenderBillboard(downTexID, region.minPoint, region.size(), region.minPoint, region.maxPoint, 0, -0.7f);  
			break;
		case GW_BTNSTATE_OVER:
			// TODO: Z depth
			renderer.RenderBillboard(overTexID, region.minPoint, region.size(), region.minPoint, region.maxPoint, 0, -0.7f);  
			break;
		default:
			assert(false); // Something went very wrong
		}
	}

	void Button::FrameStart() {
		sharedState.hitFound = false;
	}
}