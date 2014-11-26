#include "common.h"

// Game includes
#include "Game\World.h"
#include "Game\Renderer.h"
#include "Game\Game.h"
#include "Game\Input.h"

int main(int argc, char *argv[]){

	// Game systems
	gw::Input		input;
	gw::World		world;
	gw::Renderer	renderer;
	gw::Game		game;

	// Initialization
	TRACE_WARNING("Initializing..." << std::endl);
	
	if (!renderer.Initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT)) {
		TRACE_ERROR("Could not initialize Renderer, ending...");
		return 1;
	}

	if (!world.Initialize(renderer)) {
		TRACE_ERROR("Could not initialize World, ending...");
		return 2;
	}

	if (!game.Initialize()) {
		TRACE_ERROR("Could not initialize Game, ending...");
		return 3;
	}

	if (!input.Initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT)) {
		TRACE_ERROR("Could not initialize Input, ending...");
		return 4;
	}

	SDL_Event event;
	bool running = true;

	// Main Loop
	while(running){

		SDL_PumpEvents();

		// Get messages
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT){
				running = false;
				break;
			}
		}

		// Update input
		input.Tick();

		// Update game world
		world.Tick(renderer);

		// Render frame
		renderer.Render();
	} 

	// Cleanup
	input.Destroy();
	game.Destroy();
	world.Destroy();
	renderer.Destroy();

	return 0;
}

