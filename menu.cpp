#include "menu.h"

//Base menu class
Menu::Menu()
{
	bgcolor = { 90, 237, 148,0 };

}

Menu::Menu(SDL_Renderer * r)
{
	Renderer = r;
	bgcolor = { 90, 237, 148,0 };
}

Menu::~Menu()
{
	int imax = InteractiveButtons.size();
	int nmax = InfoButtons.size();

	for (int i = 0; i < imax; i++)
	{
		delete InteractiveButtons[i];
	}
	for (int i = 0; i < nmax; i++)
	{
		delete InfoButtons[i];
	}
	InfoButtons.clear();
	InteractiveButtons.clear();
	delete backButton;
}

//void Menu::handleEvent(SDL_Event * e)
//{
//	int imax = InteractiveButtons.size();
//	for (int i = 0; i < imax; i++)
//	{
//		InteractiveButtons[i]->handleEvent(e);
//	}
//}

void Menu::render()
{
	int imax = InteractiveButtons.size();
	int nmax = InfoButtons.size();

	for (int i = 0; i < imax; i++)
	{
		InteractiveButtons[i]->render();
	}
	for (int i = 0; i < nmax; i++)
	{
		InfoButtons[i]->render();
	}
	if (backButton != nullptr)
		backButton->render();
	SDL_SetRenderDrawColor(Renderer, 57, 14, 132, 0);
}

void Menu::addButton(Rectangle r, std::string text)
{
	InteractiveButtons.push_back(new menuButton(r, text, Renderer));
}

void Menu::addInfo(Rectangle r, std::string text)
{
	InfoButtons.push_back(new menuButton(r, text, Renderer));
}

ControllerMenu::ControllerMenu()
{
}

ControllerMenu::ControllerMenu(SDL_Renderer * r)
{
	Renderer = r;
	bgcolor = { 64,64,64,0 };
	Rectangle backButtonRect = Rectangle(0, 0, 100, 50);
	backButton = new menuButton(backButtonRect, "EXIT", Renderer);
}

ControllerMenu::~ControllerMenu()
{
}

void ControllerMenu::handleEvent(SDL_Event * e, SDL_Joystick* j)
{
	if (e->type == SDL_CONTROLLERBUTTONDOWN && e->cbutton.button == SDL_CONTROLLER_BUTTON_BACK) {	//toggle music with select
	//If the music is paused 
		if (Mix_PausedMusic() == 1) {
			Mix_ResumeMusic();
		}
		//If the music is playing 
		else {
			Mix_PauseMusic();
		}
	}
	int jvalX = SDL_JoystickGetAxis(j, 0);
	int jvalY = SDL_JoystickGetAxis(j, 1);
	static bool onBack = false;
	// see if the user swapped between the options list and back button
	if (jvalX < -JOYSTICK_THRESHOLD && !onBack)
	{
		onBack = true;
		InteractiveButtons.at(activeButtonIndex)->setHover(false);
		backButton->setHover(true);
	}
	else if (jvalX > JOYSTICK_THRESHOLD && onBack)
	{
		onBack = false;
		InteractiveButtons.at(activeButtonIndex)->setHover(true);
		backButton->setHover(false);
	}
	//if not on back button, check if user changed active button within the button list
	if (!onBack) {
		if (jvalY < -JOYSTICK_THRESHOLD)
		{
			if (activeButtonIndex > 0)
			{
				InteractiveButtons.at(activeButtonIndex)->setHover(false);
				--activeButtonIndex;
				InteractiveButtons.at(activeButtonIndex)->setHover(true);
			}
		}
		else if (jvalY > JOYSTICK_THRESHOLD)
		{
			if (activeButtonIndex < (int(InteractiveButtons.size()) - 1))
			{
				InteractiveButtons.at(activeButtonIndex)->setHover(false);
				++activeButtonIndex;
				InteractiveButtons.at(activeButtonIndex)->setHover(true);
			}
		}
	}
	//Pass the event for button presses
	if (onBack)
		backButton->handleEvent(e);
	if (backButton->pressed) {
		backButton->pressed = false;
		back = true;
	}
	else
	{
		InteractiveButtons.at(activeButtonIndex)->handleEvent(e);
		if (InteractiveButtons.at(0)->pressed) { // PVP BUTTON
			next = true;
			exit = true;
			InteractiveButtons.at(0)->pressed = false;
		}
		else if (InteractiveButtons.at(1)->pressed) {	//PVAI button
			exit = true;
			AIGame = true;
			InteractiveButtons.at(1)->pressed = false;
		}
	}



}

ControllerSPMenu::ControllerSPMenu()
{
}

ControllerSPMenu::ControllerSPMenu(SDL_Renderer * r)
{
	Renderer = r;
	bgcolor = { 90, 237, 148,0 };
	Rectangle backButtonRect = Rectangle(0, 0, 100, 50);
	backButton = new menuButton(backButtonRect, "BACK", Renderer);
	//initialize Rectangles for NonInteractiveButtons info buttons(set size and position)
	Rectangle titleRect = Rectangle(WINDOW_WIDTH / 2, 0, 300, 100);
	Rectangle scoreRect = Rectangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 150, 300, 100);
	Rectangle scoreLabelRect = Rectangle(WINDOW_WIDTH / 2 - 310, WINDOW_HEIGHT / 2 - 150, 300, 100);
	//add infobuttons in any order
	InfoButtons.push_back(new menuButton(titleRect, "SNAKE SNAKE OoOH ITS A SNAKE", Renderer));
	InfoButtons.push_back(new menuButton(scoreLabelRect, "Score:", Renderer));
	InfoButtons.push_back(new menuButton(scoreRect, "0", Renderer));

	//initialize rectangles for interactive Buttons
	Rectangle startRect = Rectangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300, 100);
	//add the buttons from highest on the screen->lowest
	InteractiveButtons.push_back(new menuButton(startRect, "Play Again", Renderer));
}

ControllerSPMenu::~ControllerSPMenu()
{

}

ControllerMPMenu::ControllerMPMenu()
{
}

ControllerMPMenu::ControllerMPMenu(SDL_Renderer * r)
{
	Renderer = r;
	bgcolor = { 90, 237, 148,0 };

	Rectangle backButtonRect = Rectangle(2, 2, 100, 50);
	backButton = new BackButton(backButtonRect, "EXIT", Renderer);

	Rectangle titleRect = Rectangle(WINDOW_WIDTH / 2 - 350, 0, 700, 100);
	Rectangle ControlsRect = Rectangle(WINDOW_WIDTH / 2 - 200, 200, 400, 50);
	Rectangle ButtonControlsRect = Rectangle(WINDOW_WIDTH / 2 - 200, 260, 500, 50);
	Rectangle ButtonControlsRect2 = Rectangle(WINDOW_WIDTH / 2 - 200, 320, 500, 50);

	InfoButtons.push_back(new InfoButton(titleRect, "XNTRON", Renderer));
	InfoButtons.push_back(new InfoButton(ControlsRect, "Left Joystick to turn", Renderer));
	InfoButtons.push_back(new InfoButton(ButtonControlsRect, "'X' or 'B' button to shoot", Renderer));
	InfoButtons.push_back(new InfoButton(ButtonControlsRect2, "First to 7 Wins", Renderer));

	Rectangle p1v1Rect(WINDOW_WIDTH / 2 - 100, 420, 300, 64);
	Rectangle pvaiRect(WINDOW_WIDTH / 2 - 100, 530, 300, 64);
	Rectangle musicRect(WINDOW_WIDTH / 2 - 100, 640, 500, 64);

	InteractiveButtons.push_back(new GPMenuButton(p1v1Rect, "P V P", Renderer));
	InteractiveButtons.at(0)->setHover(true);
	InteractiveButtons.push_back(new GPMenuButton(pvaiRect, "1 Player", Renderer));

	bgMusic = Mix_LoadMUS("sound/music_loop.wav");
	Mix_PlayMusic(bgMusic, -1);


}

ControllerMPMenu::~ControllerMPMenu()
{

}



Texture* CombatInfoPanel::P1AmmoTexture = NULL;
Texture* CombatInfoPanel::P2AmmoTexture = NULL;
Texture* CombatInfoPanel::PanelTexture = NULL;

CombatInfoPanel::CombatInfoPanel()
{
}
CombatInfoPanel::CombatInfoPanel(SDL_Renderer * r)
{
	Renderer = r;

	PanelTexture = TextureBank::Get("SplitPanel");
	P1AmmoTexture = TextureBank::Get("ProjectileRED");
	P2AmmoTexture = TextureBank::Get("ProjectileBLUE");

	Font = TTF_OpenFont("zero hour.ttf", 48);
	SDL_Surface* surface = TTF_RenderText_Solid(Font, "0", SDL_Color{168,0,0,0});
	if (surface == NULL)
	{
		printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	P1ScoreText = SDL_CreateTextureFromSurface(Renderer, surface);

	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(Font, "0", SDL_Color{ 0,0,168,0 });
	if (surface == NULL)
	{
		printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	P2ScoreText = SDL_CreateTextureFromSurface(Renderer, surface);

	SDL_FreeSurface(surface);
	
}

CombatInfoPanel::~CombatInfoPanel()
{
	SDL_DestroyTexture(P1ScoreText);
	P1ScoreText = NULL;
	SDL_DestroyTexture(P2ScoreText);
	P2ScoreText = NULL;
}

void CombatInfoPanel::update(int shooter, int winner)
{
	if ( shooter!= 0)
		updateAmmo(shooter);
	if (winner != 0)
		updateScore(winner);
}
void CombatInfoPanel::render()
{
	PanelTexture->Render(0, 0);
	for (int i = 0; i < P1Ammo; i++) {
		P1AmmoTexture->Render(20 + i * 60, 25);
	}
	for (int i = 0; i < P2Ammo; i++) {
		P2AmmoTexture->Render(WINDOW_WIDTH - (50 + 20 + i * 60), 25);
	}
	SDL_RenderCopy(Renderer, P1ScoreText, NULL, &P1ScoreRect);
	SDL_RenderCopy(Renderer, P2ScoreText, NULL, &P2ScoreRect);
}
void CombatInfoPanel::updateAmmo(int shooter)
{
	switch (shooter) {
	case 1:
		if (P1Ammo > 0)
			--P1Ammo;
		break;
	case 2:
		if (P2Ammo > 0)
			--P2Ammo;
		break;
	case 3:
		P1Ammo = 5;
		P2Ammo = 5;
		break;
	}
}
void CombatInfoPanel::updateScore(int winner)
{
	switch (winner) {
	case 1:
		if (P1Score < 7) {
			++P1Score;
			SDL_Surface* surface = TTF_RenderText_Solid(Font, std::to_string(P1Score).c_str(), SDL_Color{ 168,0,0,0 });
			if (surface == NULL)
			{
				printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
			}

			P1ScoreText = SDL_CreateTextureFromSurface(Renderer, surface);
			SDL_FreeSurface(surface);
		}
		break;
	case 2:
		if (P2Score < 7) {
			++P2Score;
			SDL_Surface* surface = TTF_RenderText_Solid(Font, std::to_string(P2Score).c_str(), SDL_Color{ 0,0,168,0 });
			if (surface == NULL)
			{
				printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
			}

			P2ScoreText = SDL_CreateTextureFromSurface(Renderer, surface);
			SDL_FreeSurface(surface);
		}
		break;
	}
}