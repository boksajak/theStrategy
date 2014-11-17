#include "common.h"
#include "SDL.h"


// http://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution
// Get the horizontal and vertical screen sizes in pixel (of primary monitor)
void GetDesktopResolution(int& screenWidth, int& screenHeight, int& workAreaWidth, int& workAreaHeight)
{
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	workAreaWidth = rect.right;
	workAreaHeight = rect.bottom;
}

  
int main(int argc, char *argv[]) 
{
    SDL_Init(SDL_INIT_HAPTIC);
    SDL_Quit();
    return 0;
}

