#include "Input.h"
#include "common.h"

// Include SDL
#include "SDL.h"

namespace gw {

	bool Input::Initialize(float screenWidth, float screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		return true;
	}

	void Input::Destroy(){
		
	}

	void Input::Tick(){

		// Get mouse state from SDL
		Uint8 sdlMouseState = SDL_GetMouseState(&state.mouse.x, &state.mouse.y);

		// Calculate normalized mouse position
		state.mouse.fx = (float) state.mouse.x / (screenWidth - 1.0f);
		state.mouse.fy = (float) state.mouse.y / (screenHeight - 1.0f);

		// Reset click "events"
		state.mouse.leftClick = false;
		state.mouse.middleClick = false;
		state.mouse.rightClick = false;

		// Get mouse buttons states
		if (sdlMouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (!state.mouse.leftDown) state.mouse.leftClick = true;
			state.mouse.leftDown = true;
		} else {
			state.mouse.leftDown = false;
		}

		if (sdlMouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
			if (!state.mouse.middleDown) state.mouse.middleClick = true;
			state.mouse.middleDown = true;
		} else {
			state.mouse.middleDown = false;
		}

		if (sdlMouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (!state.mouse.rightDown) state.mouse.rightClick = true;
			state.mouse.rightDown = true;
		} else {
			state.mouse.rightDown = false;
		}


	}
}