#include "common.h"
#include "Button.h"
#include "Loaders\ImageLoader.h"

namespace gw {

	bool Button::Initialize(size_t upTexID, size_t overTexID, size_t downTexID, AABB2D region) {
		this->upTexID = upTexID;
		this->overTexID = overTexID;
		this->downTexID = downTexID;

		this->region = region;
		return true;
	}

	void Button::Tick(Renderer &renderer) {

		switch (btnState) {
		case GW_BTNSTATE_UP:
			// Do nothing - assume that button is already rendered as "background" in inactive state
			break;
		case GW_BTNSTATE_DOWN:
			// TODO: Z depth
			renderer.RenderBillboard(downTexID, region.minPoint, region.size(), region.minPoint, region.maxPoint, 0, -1);  
			break;
		case GW_BTNSTATE_OVER:
			// TODO: Z depth
			renderer.RenderBillboard(overTexID, region.minPoint, region.size(), region.minPoint, region.maxPoint, 0, -1);  
			break;
		default:
			assert(false); // Something went very wrong
		}
	}
}