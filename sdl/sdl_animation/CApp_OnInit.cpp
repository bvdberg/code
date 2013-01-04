//==============================================================================
#include "CApp.h"

//==============================================================================
bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }

	if((Surf_Test = CSurface::OnLoad("myimage.bmp")) == NULL) {
		return false;
	}

	if((Surf_BG = CSurface::OnLoad("bg.bmp")) == NULL) {
		return false;
	}

	CSurface::Transparent(Surf_Test, 255, 0, 255);

	Anim_Yoshi.MaxFrames = 8;
	//Anim_Yoshi.Oscillate = true;

    return true;
}

//==============================================================================
