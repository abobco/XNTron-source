#include "Game.h"

Game::Game()
{
	Playing = false;
	PostMatch = false;
	PreMatch = false;
	PostGame = false;

	p1Score = 0;
	p2Score = 0;

	p1Ammo = 0;
	p2Ammo = 0;

	currMatch = 0;
}

Game::Game(SDL_Renderer * r, SDL_Joystick* j1, SDL_Joystick* j2)
{
	Playing = true;
	PostMatch = false;
	PreMatch = true;
	PostGame = false;

	p1Score = 0;
	p2Score = 0;

	p1Ammo = 5;
	p2Ammo = 5;

	currMatch = 0;

	Renderer = r;
	Joy1 = j1;
	Joy2 = j2;

	Player1 = new ControllerCompetitor(Renderer, j1);
	Player1->setPosition(100, 200);
	Player1->setVelAngle(1);
	Player1->setColor(SDL_Color{ 164,0,0,0 });
	Player1->setShip('r');
	Player2 = new ControllerCompetitor(Renderer, j2);
	Player2->setColor(SDL_Color{ 0,0,164,0 });
	Player2->setAngle(180);
	Player2->setShip('b');

	CollisionEngine = new CollisionHandler(Player1, Player2);

	ScorePanel = new CombatInfoPanel(Renderer);

	lasernoise = Mix_LoadWAV("laser.wav");
	if (lasernoise == NULL) {
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	explosionNoise = Mix_LoadWAV("sound/explosion.wav");
	if (explosionNoise == NULL) {
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
}


Game::~Game()
{
	delete Player1;
	delete Player2;
	delete CollisionEngine;
	Mix_FreeChunk(lasernoise);
	Mix_FreeChunk(explosionNoise);
	lasernoise = NULL;
}

void Game::render()
{
	if (PreMatch) {
		countdown();
		PreMatch = false;
	}
	if (ScorePanel != nullptr)
	{
		ScorePanel->render();
	}
	if (Player1 != nullptr && Player2 != nullptr)
	{
		Player1->render();
		Player2->render();
	}
	for (int i = 0; i < 10; i++)
	{
		if (CollisionEngine->projectiles[i] != nullptr)
			CollisionEngine->projectiles[i]->render();
	}
	if (p1Score >= 7)
	{
		Rectangle InfoRect(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 100, 500, 200);
		InfoButton WinnerButton(InfoRect, "RED WINS!", Renderer);
		SDL_Color TextCol = { 164,0,0,0 };
		WinnerButton.setTextColor(TextCol);
		WinnerButton.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(1000);
		PostMatch = true;
		MatchWinner = 1;
	}
	else if (p2Score >= 7)
	{
		Rectangle InfoRect(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 100, 500, 200);
		InfoButton WinnerButton(InfoRect, "BLUE WINS!", Renderer);
		WinnerButton.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(1000);
		PostMatch = true;
		MatchWinner = 2;
	}
}

void Game::handleEvent(SDL_Event * e)
{
	if (Player1 != nullptr && Player2 != nullptr && CollisionEngine != nullptr)
	{
		if (e->type == SDL_JOYAXISMOTION)
		{
			Player1->handleEvent(e);
			Player2->handleEvent(e);
		}
		if (e->type == SDL_CONTROLLERBUTTONDOWN)
		{
			//SDL_Point position = Player1->getPosition();
			//SDL_Point velocity = Player1->getVelocity();
			//collisionEngine->addProjectile(new Projectile(Renderer, position.x, position.y, velocity.x,velocity.y));

			if (e->cbutton.which == SDL_JoystickInstanceID(Joy1))
			{
				//Player1->handleEvent(Event);
				SDL_Point position = Player1->getPosition();
				if (p1Ammo > 0)
				{
					switch (e->cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_A:

						SDL_Point p1vel = Player1->getVelocity();
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x + p1vel.x * 10 - 10, position.y + p1vel.y * 10 - 10, p1vel.x, p1vel.y, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
						/*
					case SDL_CONTROLLER_BUTTON_B:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x + PROJ_OFFSET, position.y - 15, pVel, 0, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_X:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x - PROJ_OFFSET - 15, position.y- 15, -pVel, 0, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_Y:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x-10, position.y - PROJ_OFFSET - 10, 0, -pVel, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
						*/
					}
				}
			}
			else if (e->cbutton.which == SDL_JoystickInstanceID(Joy2))
			{
				//Player1->handleEvent(e);

				SDL_Point position = Player2->getPosition();
				if (p2Ammo > 0)
				{
					switch (e->cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_A:
						SDL_Point p2vel = Player2->getVelocity();
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x + p2vel.x * 10 - 10, position.y + p2vel.y * 10 - 10, p2vel.x, p2vel.y, 'b'));
						--p2Ammo;
						ScorePanel->update(2, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
						/*
					case SDL_CONTROLLER_BUTTON_B:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x + PROJ_OFFSET, position.y - 15, pVel, 0, 'b'));
						--p2Ammo;
						ScorePanel->update(2, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_X:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x - PROJ_OFFSET - 15, position.y - 15, -pVel, 0, 'b'));
						--p2Ammo;
						ScorePanel->update(2, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_Y:
						CollisionEngine->addProjectile(new Projectile(Renderer, position.x-10, position.y - PROJ_OFFSET - 10, 0, -pVel, 'b'));
						--p2Ammo;
						ScorePanel->update(2, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
						*/
					}
				}
			}
		}
	}
}

void Game::MoveObjects()
{
	if (Playing && Player1 != nullptr && Player2 != nullptr && CollisionEngine != nullptr)
	{
		Player1->move();
		Player2->move();


			MatchWinner = 0;

			CollisionEngine->moveProjectiles();

			MatchWinner = CollisionEngine->CheckEdgeCollisions();
			if (MatchWinner != 0)
			{
				switch (MatchWinner) {
				case 1:
					//render();
					p1Score++;
					ScorePanel->update(3, 1);
					resetGame();
					break;
				case 2:
					//render();
					p2Score++;
					ScorePanel->update(3, 2);
					resetGame();		
					break;
				}
				return;
			}
			MatchWinner = CollisionEngine->CheckPlayerCollisions();
			if (MatchWinner != 0)
			{
				switch (MatchWinner) {
				case 1:
					//render();
					p1Score++;
					ScorePanel->update(3, 1);
					resetGame();	
					break;
				case 2:
					//render();
					p2Score++;
					ScorePanel->update(3, 2);
					resetGame();	
					break;
				case 3:
					//render();
					ScorePanel->update(3, 0);
					resetGame();
					break;
				}
				return;
			}
			MatchWinner = CollisionEngine->CheckProjectileCollisions();
			if (MatchWinner != 0)
			{
				switch (MatchWinner) {
				case 1:
					//render();
					p1Score++;
					ScorePanel->update(3, 1);
					resetGame();	
					break;
				case 2:
					//render();
					p2Score++;
					ScorePanel->update(3, 2);
					resetGame();	
					break;
				}
				return;
			}
	}
}

void Game::resetGame()
{
	Mix_PlayChannel(-1, explosionNoise, 0);
	SDL_Delay(500);
	delete Player1, Player2, CollisionEngine;
	Player1 = new ControllerCompetitor(Renderer, Joy1);
	Player1->setPosition(100, 200);
	Player1->setVelAngle(1);
	Player1->setColor(SDL_Color{ 164,0,0,0 });
	Player2 = new ControllerCompetitor(Renderer, Joy2);
	Player2->setColor(SDL_Color{ 0,0,164,0 });
	Player2->setAngle(180);
	Player2->setShip('b');

	p1Ammo = 5;
	p2Ammo = 5;

	CollisionEngine = new CollisionHandler(Player1, Player2);

	SDL_RenderClear(Renderer);
	TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	Player1->render();
	Player2->render();
	ScorePanel->render();
	for (int i = 0; i < 10; i++)
	{
		if (CollisionEngine->projectiles[i] != nullptr)
			CollisionEngine->projectiles[i]->render();
	}
	if (p1Score < 7 && p2Score < 7) {
		Rectangle ReadyRect(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 320, 400, 200);
		InfoButton ReadyButton(ReadyRect, "Get Ready!", Renderer);
		if (MatchWinner == 1) ReadyButton.setTextColor(SDL_Color{ 168,0,0,0 });
		ReadyButton.render();

		Rectangle InfoRect3(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton3(InfoRect3, "3", Renderer);
		if (MatchWinner == 1) WinnerButton3.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton3.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect2(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton2(InfoRect2, "2", Renderer);
		if (MatchWinner == 1) WinnerButton2.setTextColor(SDL_Color{ 168,0,0,0 });
		TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		Player1->render();
		Player2->render();
		ScorePanel->render();
		for (int i = 0; i < 10; i++)
		{
			if (CollisionEngine->projectiles[i] != nullptr)
				CollisionEngine->projectiles[i]->render();
		}
		WinnerButton2.render();
		ReadyButton.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect1(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton1(InfoRect1, "1", Renderer);
		if (MatchWinner == 1) WinnerButton1.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton1.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);
	}
	MatchWinner = 0;
}

void Game::countdown()
{
	SDL_RenderClear(Renderer);
	TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	Player1->render();
	Player2->render();
	ScorePanel->render();
	for (int i = 0; i < 10; i++)
	{
		if (CollisionEngine->projectiles[i] != nullptr)
			CollisionEngine->projectiles[i]->render();
	}
	if (p1Score < 7 && p2Score < 7) {
		Rectangle ReadyRect(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 320, 400, 200);
		InfoButton ReadyButton(ReadyRect, "Get Ready!", Renderer);
		if (MatchWinner == 1) ReadyButton.setTextColor(SDL_Color{ 168,0,0,0 });
		ReadyButton.render();

		Rectangle InfoRect3(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton3(InfoRect3, "3", Renderer);
		if (MatchWinner == 1) WinnerButton3.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton3.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect2(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton2(InfoRect2, "2", Renderer);
		if (MatchWinner == 1) WinnerButton2.setTextColor(SDL_Color{ 168,0,0,0 });
		TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		Player1->render();
		Player2->render();
		ScorePanel->render();
		for (int i = 0; i < 10; i++)
		{
			if (CollisionEngine->projectiles[i] != nullptr)
				CollisionEngine->projectiles[i]->render();
		}
		WinnerButton2.render();
		ReadyButton.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect1(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton1(InfoRect1, "1", Renderer);
		if (MatchWinner == 1) WinnerButton1.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton1.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);
	}
}

AIGame::AIGame()
{
}

AIGame::AIGame(SDL_Renderer * r, SDL_Joystick * j)
{
	Playing = true;
	PostMatch = false;
	PreMatch = true;
	PostGame = false;

	p1Score = 0;
	p2Score = 0;

	p1Ammo = 5;
	p2Ammo = 5;

	currMatch = 0;

	Renderer = r;
	Joy1 = j;

	Player1 = new ControllerCompetitor(Renderer, j);
	Player1->setPosition(100, 200);
	Player1->setVelAngle(1);
	Player1->setColor(SDL_Color{ 164,0,0,0 });
	Player1->setShip('r');
	AIPlayer = new AICompetitor(Renderer);
	AIPlayer->setColor(SDL_Color{ 0,0,164,0 });
	AIPlayer->setAngle(180);
	AIPlayer->setShip('b');

	SPCollisionEngine = new SPCollisionHandler(Player1, AIPlayer);

	ScorePanel = new CombatInfoPanel(Renderer);

	lasernoise = Mix_LoadWAV("laser.wav");
	if (lasernoise == NULL) {
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	explosionNoise = Mix_LoadWAV("sound/explosion.wav");
	if (explosionNoise == NULL) {
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

AIGame::~AIGame()
{
}

void AIGame::handleEvent(SDL_Event * e)
{
	if (Player1 != nullptr && AIPlayer != nullptr && SPCollisionEngine != nullptr)
	{
		if (e->type == SDL_JOYAXISMOTION)
		{
			Player1->handleEvent(e);
		}
		if (e->type == SDL_CONTROLLERBUTTONDOWN)
		{
			//SDL_Point position = Player1->getPosition();
			//SDL_Point velocity = Player1->getVelocity();
			//collisionEngine->addProjectile(new Projectile(Renderer, position.x, position.y, velocity.x,velocity.y));

			if (e->cbutton.which == SDL_JoystickInstanceID(Joy1))
			{
				//Player1->handleEvent(Event);
				SDL_Point position = Player1->getPosition();
				if (p1Ammo > -100)
				{
					switch (e->cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_A:

						SDL_Point p1vel = Player1->getVelocity();
						SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x + p1vel.x * 10 - 10, position.y + p1vel.y * 10 -10, p1vel.x, p1vel.y, 'r'));						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_B:
						SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x + PROJ_OFFSET - 5, position.y - 15, pVel, 0, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_X:
						SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x - PROJ_OFFSET - 10, position.y - 15, -pVel, 0, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					case SDL_CONTROLLER_BUTTON_Y:
						SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x - 10, position.y - PROJ_OFFSET - 10, 0, -pVel, 'r'));
						--p1Ammo;
						ScorePanel->update(1, 0);
						Mix_PlayChannel(-1, lasernoise, 0);
						break;
					}
				}
			}		
		}
	}
	if (AIPlayer != nullptr && AIPlayer->getShoot() != 0)
	{
		SDL_Point position = AIPlayer->getPosition();
		int direction = AIPlayer->getShoot();
		switch (direction)	{
		case 1:
			SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x + PROJ_OFFSET - 5, position.y - 15, pVel, 0, 'b'));
			//--p2Ammo;
			//ScorePanel->update(2, 0);
			AIPlayer->setShoot(0);
			Mix_PlayChannel(-1, lasernoise, 0);
			break;
		case 2:
			SPCollisionEngine->addProjectile(new Projectile(Renderer, position.x - PROJ_OFFSET - 10, position.y - 15, -pVel, 0, 'b'));
			//--p2Ammo;
			//ScorePanel->update(2, 0);
			AIPlayer->setShoot(0);
			Mix_PlayChannel(-1, lasernoise, 0);
		}
		
	}
}

void AIGame::MoveObjects()
{
	if (Playing && Player1 != nullptr && AIPlayer!= nullptr && SPCollisionEngine != nullptr)
	{
		Player1->move();
		AIPlayer->move();


		MatchWinner = 0;

		SPCollisionEngine->moveProjectiles();

		MatchWinner = SPCollisionEngine->CheckEdgeCollisions();
		if (MatchWinner != 0)
		{
			switch (MatchWinner) {
			case 1:
				render();
				p1Score++;
				ScorePanel->update(3, 1);
				resetGame();
				break;
			case 2:
				render();
				p2Score++;
				ScorePanel->update(3, 2);
				resetGame();	
				break;
			}
			return;
		}
		MatchWinner = SPCollisionEngine->CheckPlayerCollisions();
		if (MatchWinner != 0)
		{
			switch (MatchWinner) {
			case 1:
				render();
				p1Score++;
				ScorePanel->update(3, 1);
				resetGame();	
				break;
			case 2:
				render();
				p2Score++;
				ScorePanel->update(3, 2);
				resetGame();	
				break;

			}
			return;
		}
		MatchWinner = SPCollisionEngine->CheckProjectileCollisions();
		if (MatchWinner != 0)
		{
			switch (MatchWinner) {
			case 1:
				render();
				p1Score++;
				ScorePanel->update(3, 1);
				resetGame();
				break;
			case 2:
				render();
				p2Score++;
				ScorePanel->update(3, 2);
				resetGame();
				break;
			}
			return;
		}
	}
}

void AIGame::resetGame()
{
	Mix_PlayChannel(-1, explosionNoise, 0);
	SDL_Delay(500);
	delete Player1, AIPlayer, SPCollisionEngine;
	Player1 = new ControllerCompetitor(Renderer, Joy1);
	Player1->setPosition(100, 200);
	Player1->setVelAngle(1);
	Player1->setColor(SDL_Color{ 164,0,0,0 });
	AIPlayer = new AICompetitor(Renderer);
	AIPlayer->setColor(SDL_Color{ 0,0,164,0 });
	AIPlayer->setAngle(180);
	AIPlayer->setShip('b');

	p1Ammo = 5;
	p2Ammo = 5;

	SPCollisionEngine = new SPCollisionHandler(Player1, AIPlayer);

	SDL_RenderClear(Renderer);
	TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	Player1->render();
	AIPlayer->render();
	ScorePanel->render();
	for (int i = 0; i < 10; i++)
	{
		if (SPCollisionEngine->projectiles[i] != nullptr)
			SPCollisionEngine->projectiles[i]->render();
	}
	if (p1Score < 7 && p2Score < 7) {
		Rectangle ReadyRect(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 320, 400, 200);
		InfoButton ReadyButton(ReadyRect, "Get Ready!", Renderer);
		if (MatchWinner == 1) ReadyButton.setTextColor(SDL_Color{ 168,0,0,0 });
		ReadyButton.render();
		Rectangle InfoRect3(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton3(InfoRect3, "3", Renderer);
		if (MatchWinner == 1) WinnerButton3.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton3.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect2(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton2(InfoRect2, "2", Renderer);
		if (MatchWinner == 1) WinnerButton2.setTextColor(SDL_Color{ 168,0,0,0 });
		TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		ScorePanel->render();
		Player1->render();
		AIPlayer->render();
		WinnerButton2.render();
		ReadyButton.render();
		for (int i = 0; i < 10; i++)
		{
			if (SPCollisionEngine->projectiles[i] != nullptr)
				SPCollisionEngine->projectiles[i]->render();
		}
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect1(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton1(InfoRect1, "1", Renderer);
		if (MatchWinner == 1) WinnerButton1.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton1.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);
	}
	MatchWinner = 0;
}

void AIGame::render()
{
	if (PreMatch) {
		countdown();
		PreMatch = false;
	}
	if (ScorePanel != nullptr)
	{
		ScorePanel->render();
	}
	if (Player1 != nullptr && AIPlayer != nullptr)
	{
		Player1->render();
		AIPlayer->render();
	}
	for (int i = 0; i < 10; i++)
	{
		if (SPCollisionEngine->projectiles[i] != nullptr)
			SPCollisionEngine->projectiles[i]->render();
	}
	if (p1Score >= 7)
	{
		Rectangle InfoRect(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 100, 500, 200);
		InfoButton WinnerButton(InfoRect, "RED WINS!", Renderer);
		SDL_Color TextCol = { 164,0,0,0 };
		WinnerButton.setTextColor(TextCol);
		WinnerButton.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(1000);
		PostMatch = true;
		MatchWinner = 1;
	}
	else if (p2Score >= 7)
	{
		Rectangle InfoRect(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 100, 500, 200);
		InfoButton WinnerButton(InfoRect, "BLUE WINS!", Renderer);
		WinnerButton.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(1000);
		PostMatch = true;
		MatchWinner = 2;
	}
}

void AIGame::countdown()
{
	SDL_RenderClear(Renderer);
	TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	Player1->render();
	AIPlayer->render();
	ScorePanel->render();
	for (int i = 0; i < 10; i++)
	{
		if (SPCollisionEngine->projectiles[i] != nullptr)
			SPCollisionEngine->projectiles[i]->render();
	}
	if (p1Score < 7 && p2Score < 7) {
		Rectangle ReadyRect(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 320, 400, 200);
		InfoButton ReadyButton(ReadyRect, "Get Ready!", Renderer);
		if (MatchWinner == 1) ReadyButton.setTextColor(SDL_Color{ 168,0,0,0 });
		ReadyButton.render();
		Rectangle InfoRect3(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton3(InfoRect3, "3", Renderer);
		if (MatchWinner == 1) WinnerButton3.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton3.render();
		ScorePanel->render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect2(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton2(InfoRect2, "2", Renderer);
		if (MatchWinner == 1) WinnerButton2.setTextColor(SDL_Color{ 168,0,0,0 });
		TextureBank::Get("021 True Sunset")->Render(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		ScorePanel->render();
		Player1->render();
		AIPlayer->render();
		WinnerButton2.render();
		ReadyButton.render();
		for (int i = 0; i < 10; i++)
		{
			if (SPCollisionEngine->projectiles[i] != nullptr)
				SPCollisionEngine->projectiles[i]->render();
		}
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);

		Rectangle InfoRect1(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200);
		InfoButton WinnerButton1(InfoRect1, "1", Renderer);
		if (MatchWinner == 1) WinnerButton1.setTextColor(SDL_Color{ 168,0,0,0 });
		WinnerButton1.render();
		SDL_RenderPresent(Renderer);
		SDL_Delay(500);
	}
}
