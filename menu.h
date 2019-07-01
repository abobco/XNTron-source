#pragma once
#include <SDL.h>
#include "menuButton.h"
#include <vector>
#include <string>
#include <iostream>
#include <SDL_mixer.h>

#ifndef __MAINMENU_H__
#define __MAINMENU_H__

class Menu
{
protected:
	static const int JOYSTICK_THRESHOLD = 4000;

	SDL_Renderer* Renderer = NULL;
	Mix_Music* bgMusic = NULL;

	std::vector<menuButton*> InteractiveButtons;
	std::vector<menuButton*> InfoButtons;
	menuButton* backButton = nullptr;

	SDL_Color bgcolor;
	
	bool exit = false;
	bool next = false;
	bool back = false;
	bool AIGame = false;

public:
	Menu();
	Menu(SDL_Renderer* r);
	~Menu();

	virtual void handleEvent(SDL_Event* e, SDL_Joystick* j)=0;

	void render();

	void addButton(Rectangle r, std::string text);
	void addInfo(Rectangle r, std::string text);

	bool checkExit() { return exit; }
	void setExit(bool e) { exit = e; }
	bool checkNext() { return next; }
	void setNext(bool n) { next = n; }
	bool checkBack() { return back; }
	void setBack(bool b) { back = b; }
	bool checkAIGame() { return AIGame; }
	void setAIGame(bool a) { AIGame = a; }
};

class ControllerMenu : public Menu
{
protected:
	int activeButtonIndex = 0;

public:
	ControllerMenu();
	ControllerMenu(SDL_Renderer* r);
	~ControllerMenu();

	void handleEvent(SDL_Event* e, SDL_Joystick* j);
};

class ControllerSPMenu : public ControllerMenu
{
public:
	ControllerSPMenu();
	ControllerSPMenu(SDL_Renderer* r);
	~ControllerSPMenu();
};

class ControllerMPMenu : public ControllerMenu
{
public:
	ControllerMPMenu();
	ControllerMPMenu(SDL_Renderer* r);
	~ControllerMPMenu();
};

class CombatInfoPanel {
private:
	int P1Score = 0;
	int P2Score = 0;

	int P1Ammo = 5;
	int P2Ammo = 5;

	static Texture* P1AmmoTexture;
	static Texture* P2AmmoTexture;
	static Texture* PanelTexture;

	SDL_Texture* P1ScoreText;
	SDL_Texture* P2ScoreText;
	SDL_Rect P1ScoreRect = { WINDOW_WIDTH / 2 - (50+36), 20, 36, 80 };
	SDL_Rect P2ScoreRect = { WINDOW_WIDTH / 2 + 50, 20, 36, 80 };


	TTF_Font* Font;
	SDL_Renderer* Renderer;

private:
	void updateAmmo(int shooter);
	void updateScore(int winner);
	
public:
	CombatInfoPanel();
	CombatInfoPanel(SDL_Renderer* r);
	~CombatInfoPanel();

	void update(int shooter, int winner);

	void render();




};
#endif