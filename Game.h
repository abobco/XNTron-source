#pragma once

#include "CollisionHandler.h"
#include "menu.h"
#include <SDL_mixer.h>

#ifndef  __GAME_H__
#define  __GAME_H__

class Game
{
protected:
	SDL_Renderer* Renderer = NULL;
	SDL_Joystick* Joy1;
	SDL_Joystick* Joy2;

	CombatInfoPanel* ScorePanel = nullptr;
	Mix_Chunk* lasernoise = NULL;
	Mix_Chunk* explosionNoise = NULL;


	int p1Score;
	int p2Score;

	int p1Ammo;
	int p2Ammo;

	const int PROJ_OFFSET = 30;
	const int pVel = 10;

	int currMatch;
	const int MAX_MATCHES = 7;
	int MatchWinner = 0;

	bool Playing;
	bool PostMatch;
	bool PreMatch;
	bool PostGame;


public:
	CollisionHandler* CollisionEngine;
	ControllerCompetitor* Player1;
	ControllerCompetitor* Player2;

	enum Winner {
		NONE,
		PLAYER1,
		PLAYER2,
		PLAYER3,
		PLAYER4
	};
public:
	Game();
	Game(SDL_Renderer* r, SDL_Joystick* j1, SDL_Joystick* j2);
	~Game();

	void render();
	void handleEvent(SDL_Event* e);
	void MoveObjects();
	void resetGame();
	void countdown();

	int GetWinner() { return MatchWinner; }
};

class AIGame : public Game {
public:
	AICompetitor* AIPlayer;

	SPCollisionHandler* SPCollisionEngine;
public:
	AIGame();
	AIGame(SDL_Renderer* r, SDL_Joystick* j);
	~AIGame();
	void handleEvent(SDL_Event* e);
	void MoveObjects();
	void resetGame();
	void render();
	void countdown();
};
#endif // ! __GAME_H__