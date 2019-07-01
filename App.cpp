//==============================================================================
#include "App.h"
#include "Log.h"

App App::Instance;

//==============================================================================
App::App() {
}

//------------------------------------------------------------------------------
void App::OnEvent(SDL_Event* Event) {
	if (Event->key.keysym.sym == SDLK_ESCAPE) {
		Running = false;
		return;
	}
	if (!MPmenu->checkExit())
	{
		MPmenu->handleEvent(Event, Joy1);
		if (MPmenu->checkExit() && MPmenu->checkNext())
			PVPGame = new Game(Renderer, Joy1, Joy2);
		if (MPmenu->checkExit() && MPmenu->checkAIGame())
			AIgame = new AIGame(Renderer, Joy1);
		if (!MPmenu->checkExit() && MPmenu->checkBack()) {
			Running = false;
			return;
		}
	}
	else if (MPmenu->checkNext())
	{
		PVPGame->handleEvent(Event);
	}
	else if (MPmenu->checkAIGame()) {
		AIgame->handleEvent(Event);
	}
}

//------------------------------------------------------------------------------
bool App::Init() {
	if (!InitSDL())
		return false;
	MPmenu = new ControllerMPMenu(Renderer);

	GP1 = SDL_GameControllerOpen(0);
	Joy1 = SDL_GameControllerGetJoystick(GP1);
	GP2 = SDL_GameControllerOpen(1);
	Joy2 = SDL_GameControllerGetJoystick(GP2);

	return true;
}
bool App::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
		Log("Unable to Init SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		Log("Unable to Init hinting: %s", SDL_GetError());
	}

	if ((Window = SDL_CreateWindow(
		"XNTron",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WindowWidth, WindowHeight, SDL_WINDOW_SHOWN)
		) == NULL) {
		Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}
	SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);
	PrimarySurface = SDL_GetWindowSurface(Window);

	if ((Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC)) == NULL) {
		Log("Unable to create renderer");
		return false;
	}

	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);

	// Initialize image loading for PNGs
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		Log("Unable to init SDL_image: %s", IMG_GetError());
		return false;
	}

	// Load all of our Textures (see TextureBank class for expected folder)
	if (TextureBank::Init() == false) {
		Log("Unable to init TextureBank");
		return false;
	}

	if (TTF_Init() != 0)
	{
		Log("Unable to init SDL_ttf: %s", TTF_GetError());
	}
	//Initialize SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 ) { 
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); 
		return false; 
	}

	if (SDLNet_Init() == -1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}


	//170161__timgormly__8-bit-laser

	return true;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void App::Loop() {
	if (MPmenu->checkNext())
		PVPGame->MoveObjects();
	else if (MPmenu->checkAIGame())
		AIgame->MoveObjects();
}

//------------------------------------------------------------------------------
void App::Render() {
	SDL_RenderClear(Renderer);
	SDL_SetRenderDrawColor(Renderer, 90, 237, 148, 0);
	//TextureBank::Get("xabnab")->Render(0, 0); // You should really check your pointers
	TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!MPmenu->checkExit())
		MPmenu->render();
	else if (MPmenu->checkNext())
	{
		
		PVPGame->render();
		if (PVPGame->GetWinner() != 0)
		{
			int winner = PVPGame->GetWinner();
			switch (winner){
			case 1:
				RedWins++;
				break;
			case 2:
				BlueWins++;
				break;
			}
			MPmenu->setExit(false);
			MPmenu->setNext(false);
		}
		SDL_SetRenderDrawColor(Renderer, 128, 190, 128, 0);
	}
	else if (MPmenu->checkAIGame()) {
		AIgame->render();
		if (AIgame->GetWinner() != 0)
		{
			int winner = AIgame->GetWinner();
			switch (winner) {
			case 1:
				RedWins++;
				break;
			case 2:
				BlueWins++;
				break;
			}
			MPmenu->setExit(false);
			MPmenu->setNext(false);
			MPmenu->setAIGame(false);
		}
	}

	SDL_RenderPresent(Renderer);
}

//------------------------------------------------------------------------------
void App::Cleanup() {
	TextureBank::Cleanup();

	if(Renderer) {
		SDL_DestroyRenderer(Renderer);
		Renderer = NULL;
	}

	if (Window) {
		SDL_DestroyWindow(Window);
		Window = NULL;
	}
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	SDLNet_Quit();
	//delete PVPGame;
}

//------------------------------------------------------------------------------
int App::Execute(int argc, char* argv[]) {
	if(!Init()) return 0;

	SDL_Event Event;

	while(Running) {
		while(SDL_PollEvent(&Event) != 0) {
			OnEvent(&Event);

			if(Event.type == SDL_QUIT) Running = false;
		}

		if (Running) {
			Render();
			Loop();
			

			SDL_Delay(1); // Breath
		}
	}

	Cleanup();

	return 1;
}

//==============================================================================
SDL_Renderer* App::GetRenderer() { return Renderer; }

//==============================================================================
App* App::GetInstance() { return &App::Instance; }

int App::GetWindowWidth()  { return WindowWidth; }
int App::GetWindowHeight() { return WindowHeight; }

//==============================================================================
