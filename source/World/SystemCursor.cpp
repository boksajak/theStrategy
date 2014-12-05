#include "common.h"
#include "SystemCursor.h"
#include "Loaders\ImageLoader.h"

// Include SDL
#include "SDL.h"

namespace gw {

	SystemCursorSharedState SystemCursor::sharedCursorState;

	SystemCursor::~SystemCursor() {

		if (upCursor != NULL) {
			SDL_FreeCursor(upCursor);
			upCursor = NULL;
		}

		if (upSurface != NULL) {
			SDL_FreeSurface(upSurface);
			upSurface = NULL;
		}

		if (downCursor != NULL) {
			SDL_FreeCursor(downCursor);
			downCursor = NULL;
		}

		if (downSurface != NULL) {
			SDL_FreeSurface(downSurface);
			downSurface = NULL;
		}

		if (upImg.bytes != NULL) {

			if (downImg.bytes == upImg.bytes) {
				downImg.bytes = NULL;
			}

			delete[] upImg.bytes;
			upImg.bytes = NULL;
		}

		if (downImg.bytes != NULL) {
			delete[] downImg.bytes;
			downImg.bytes = NULL;
		}

	}

	bool SystemCursor::Initialize(const Image &upTexture, const Image &downTexture, glm::vec2 size, glm::vec2 screenSize) {
	
		this->size = absoulteToNormalized(size, screenSize);
		this->upImg = upTexture;
		this->downImg = downTexture;

		if (!LoadSDLCursor(upImg, upSurface, upCursor)) {
			return false;
		}

		if (!LoadSDLCursor(downImg, downSurface, downCursor)) {
			return false;
		}


		return true;
	}

	void SystemCursor::Update(Input &input) {
		if (input.state.mouse.leftDown) {
			cursorState = GW_CRSRSTATE_DOWN;
		} else {
			cursorState = GW_CRSRSTATE_UP;
		}
	}

	void SystemCursor::Render(Renderer &renderer) {

		if (sharedCursorState.currentCursor != this || sharedCursorState.state != cursorState) {
			sharedCursorState.currentCursor = this;
			sharedCursorState.state = cursorState;

			switch (cursorState) {
			case GW_CRSRSTATE_UP:
				SDL_SetCursor(upCursor);
				break;
			case GW_CRSRSTATE_DOWN:
				SDL_SetCursor(downCursor);
				break;
			default:
				assert(false); // something went very wrong
			}
		}
	}

	bool SystemCursor::LoadSDLCursor(const Image& image, SDL_Surface* &surface, SDL_Cursor* &cursor) {
		Uint32 rmask, gmask, bmask, amask;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif

		surface = SDL_CreateRGBSurfaceFrom(image.bytes, image.width, image.height, image.bitsPerPixel, image.width * image.bitsPerPixel / 8, rmask, gmask, bmask, amask);

		if(surface == NULL) {
			TRACE_ERROR("CreateRGBSurface failed: " <<  SDL_GetError() << std::endl);
			return false;
		}

		cursor = SDL_CreateColorCursor(surface, 0, 0);

		if(cursor == NULL) {
			TRACE_ERROR("SDL_CreateColorCursor failed: " <<  SDL_GetError() << std::endl);
			return false;
		}
	}


}