#include "common.h"

// Game includes
#include "Game\World.h"
#include "Game\Renderer.h"
#include "Game\Game.h"

int main(int argc, char *argv[]){

	// Game systems
	gw::World		world;
	gw::Renderer	renderer;
	gw::Game		game;

	// Initialization
	TRACE_WARNING("Initializing..." << std::endl);

	if (!world.Initialize()) {
		TRACE_ERROR("Could not initialize World, ending...");
		return 1;
	}

	if (!renderer.Initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT)) {
		TRACE_ERROR("Could not initialize Renderer, ending...");
		return 2;
	}

	if (!game.Initialize()) {
		TRACE_ERROR("Could not initialize Game, ending...");
		return 3;
	}

	SDL_Event event;
	bool running = true;

	// Main Loop
	while(running){

		// Get messages
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT){
				running = false;
				break;
			}
		}

		// Render
		renderer.Render(world);
	} 

	// Cleanup
	game.Destroy();
	renderer.Destroy();
	world.Destroy();

	return 0;
}

