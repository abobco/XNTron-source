//==============================================================================
//==============================================================================
#ifndef __APP_H__
	#define __APP_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include "CAnimation.h"
#include "TextureBank.h"
#include "macros.h"
#include "menu.h"
#include "Game.h"

class App {
	private:
		static App Instance;

		bool Running = true;

		SDL_Window* Window = NULL;
		SDL_Renderer* Renderer = NULL;
		SDL_Surface* PrimarySurface = NULL;

		static const int WindowWidth = WINDOW_WIDTH;
		static const int WindowHeight = WINDOW_HEIGHT;

		Texture* TestTexture = NULL;

		ControllerMPMenu* MPmenu = NULL;

		SDL_GameController* GP1 = NULL;
		SDL_Joystick* Joy1 = NULL;
		SDL_GameController* GP2 = NULL;
		SDL_Joystick* Joy2 = NULL;

		Game* PVPGame = NULL;
		AIGame* AIgame = NULL;

	private:
		App();

		// Capture SDL Events
		void OnEvent(SDL_Event* Event);

		// Initialize our SDL game / app
		bool Init();

		bool InitSDL();

		// Logic loop
		void Loop();

		// Render loop (draw)
		void Render();

		// Free up resources
		void Cleanup();

		//keeps track of wins over the session
		int RedWins;
		int BlueWins;

	public:
		int Execute(int argc, char* argv[]);

	public:
		SDL_Renderer* GetRenderer();

	public:
		static App* GetInstance();

		static int GetWindowWidth();
		static int GetWindowHeight();
};

#endif
