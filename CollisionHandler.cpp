#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
	for (int i = 0; i < 10; i++)
	{
		projectiles[i] = nullptr;
	}
}
CollisionHandler::CollisionHandler(ControllerCompetitor* p1, ControllerCompetitor* p2)
{
	player1 = p1;
	player2 = p2;

	for (int i = 0; i < 10; i++)
	{
		projectiles[i] = nullptr;
	}
}
CollisionHandler::~CollisionHandler()
{
	for (int i = 0; i < 10; i++)
	{
		delete projectiles[i];
		projectiles[i] = nullptr;
	}
	delete player1;
	delete player2;
}

int CollisionHandler::CheckEdgeCollisions()
{
	for (int i = 0; i < shotProjectiles; i++)
	{
		if (projectiles[i] != nullptr && !SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &killBox))
		{
			delete projectiles[i];
			projectiles[i] = nullptr;
		}
	}

	if (!SDL_HasIntersection(&player1->simRect.getRect(),&killBox))
		return 2; //player 2 wins!
	if (!SDL_HasIntersection(&player2->simRect.getRect(), &killBox))
		return 1; //player 1 wins!

	//if nobody died
	return 0;
}



bool CollisionHandler::CheckAngledCollision(Rectangle A, Rectangle B)
{



	return false;
}

int CollisionHandler::CheckPlayerCollisions()
{
	//check for collision between the 2 players
	if (SDL_HasIntersection(&player1->simRect.getRect(), &player2->simRect.getRect()))
		return 3; // its a draw!

	//check for collision between p1 and p2 tail
	for (int i = 0; i < player2->getLiveParticles(); i++) 
	{
		if ( player2->tail[i] != nullptr && SDL_HasIntersection( &player1->simRect.getRect(), &player2->tail[i]->Rect.getRect() ) )
			return 2; //Player 2 wins!
	}
	//check for collision between p1 and p1 tail
	for (int i = 0; i < player1->getLiveParticles(); i++) 
	{
		if (player1->tail[i] != nullptr && SDL_HasIntersection(&player1->simRect.getRect(), &player1->tail[i]->Rect.getRect()) && player1->tail[i]->getFrame() > 10)
			return 2; //Player 2 wins!
	}

	//check for collision between p2 and p1 tail
	for (int i = 0; i < player1->getLiveParticles(); i++)
	{
		if (player1->tail[i] != nullptr && SDL_HasIntersection(&player2->simRect.getRect(), &player1->tail[i]->Rect.getRect()))
			return 1; //Player 1 wins!
	}
	// between p2 and p2 tail
	for (int i = 0; i < player2->getLiveParticles(); i++)
	{
		if (player2->tail[i] != nullptr && SDL_HasIntersection(&player2->simRect.getRect(), &player2->tail[i]->Rect.getRect()) && player2->tail[i]->getFrame() > 10)
			return 1; //Player 1 wins!
	}

	//if no collisions
	return 0;
}

int CollisionHandler::CheckProjectileCollisions()
{
	for (int i = 0; i < 10; i++)
	{
		if (projectiles[i] != nullptr)
		{
			////check for player kills
			//SDL_Rect tColRect = projectiles[i]->collisionRect.getRect();
			//tColRect.x += 10;
			//tColRect.y += 10;
			//tColRect.w -= 20;
			//tColRect.h -= 20;
			//if (SDL_HasIntersection(&tColRect, &player1->simRect.getRect()))
			//	return 2; // player 2 wins!
			//if (SDL_HasIntersection(&tColRect, &player2->simRect.getRect()))
			//	return 1; //player 1 wins!

			SDL_Rect txRect = projectiles[i]->CollisionX.getRect();
			SDL_Rect tyRect = projectiles[i]->CollisionY.getRect();

			if (SDL_HasIntersection(&txRect, &player1->simRect.getRect()) || SDL_HasIntersection(&tyRect, &player1->simRect.getRect()))
				return 2; // player 2 wins!
			if (SDL_HasIntersection(&txRect, &player2->simRect.getRect()) || SDL_HasIntersection(&tyRect, &player2->simRect.getRect()))
				return 1; //player 1 wins!

			//check for projectile on tail collisions and delete portions of the tail
			for (int j = 0; j < player1->getLiveParticles(); j++)
			{
				if (player1->tail[j] != nullptr && SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &player1->tail[j]->Rect.getRect()))
				{
					delete player1->tail[j];
					player1->tail[j] = nullptr;
				}
			}
			for (int j = 0; j < player2->getLiveParticles(); j++)
			{
				if (player2->tail[j] != nullptr && SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &player2->tail[j]->Rect.getRect()))
				{
					delete player2->tail[j];
					player2->tail[j] = nullptr;
				}
			}
			//check for collisions with other projectiles
			for (int i = 0; i < 10; i++) {
				if (projectiles[i] != nullptr) {
					for (int j = 0; j < 10; j++) {
						if (projectiles[j] != nullptr && j != i) {
							if (SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &projectiles[j]->collisionRect.getRect())) {
								delete projectiles[i], projectiles[j];
								projectiles[i] = projectiles[j] = nullptr;
							}
						}
					}
				}
			}
		}
	}
	//if nobody died
	return 0;
}

void CollisionHandler::addProjectile(Projectile* p)
{
	if (shotProjectiles < 10)
	{
		if (projectiles[shotProjectiles] == nullptr)
		{
			delete projectiles[shotProjectiles];
			projectiles[shotProjectiles] = p;
			shotProjectiles++;
		}
	}
}

void CollisionHandler::moveProjectiles()
{
	for (int i = 0; i < 10; i++)
	{
		if (projectiles[i] != nullptr)
			projectiles[i]->move();
	}
}



SPCollisionHandler::SPCollisionHandler()
{
}

SPCollisionHandler::SPCollisionHandler(ControllerCompetitor * p1, AICompetitor * AIp)
{
	player1 = p1;
	AIPlayer = AIp;

	for (int i = 0; i < 10; i++)
	{
		projectiles[i] = nullptr;
	}
}

SPCollisionHandler::~SPCollisionHandler()
{
}

void SPCollisionHandler::addProjectile(Projectile * p)
{
	if (shotProjectiles < 10)
	{
		if (projectiles[shotProjectiles] == nullptr)
		{
			delete projectiles[shotProjectiles];
			projectiles[shotProjectiles] = p;
			shotProjectiles++;
		}
	}
	else
	{
		shotProjectiles = 0;
		projectiles[shotProjectiles] = p;
		shotProjectiles++;
	}
}

int SPCollisionHandler::CheckEdgeCollisions()
{
	for (int i = 0; i < shotProjectiles; i++)
	{
		if (projectiles[i] != nullptr && !SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &killBox))
		{
			delete projectiles[i];
			projectiles[i] = nullptr;
		}
	}

	if (!SDL_HasIntersection(&player1->simRect.getRect(), &killBox))
		return 2; //player 2 wins!
	if (!SDL_HasIntersection(&AIPlayer->simRect.getRect(), &killBox))
		return 1; //player 1 wins!

	//if nobody died
	return 0;
}

int SPCollisionHandler::CheckPlayerCollisions()
{
	//check for collision between p1 and p2 tail
	for (int i = 0; i < AIPlayer->getLiveParticles(); i++)
	{
		if (AIPlayer->tail[i] != nullptr && SDL_HasIntersection(&player1->simRect.getRect(), &AIPlayer->tail[i]->Rect.getRect()))
			return 2; //Player 2 wins!
	}
	//check for collision between p1 and p1 tail
	for (int i = 0; i < player1->getLiveParticles(); i++)
	{
		if (player1->tail[i] != nullptr && SDL_HasIntersection(&player1->simRect.getRect(), &player1->tail[i]->Rect.getRect()) && player1->tail[i]->getFrame() > 10)
			return 2; //Player 2 wins!
	}

	//check for collision between p2 and p1 tail
	for (int i = 0; i < player1->getLiveParticles(); i++)
	{
		if (player1->tail[i] != nullptr && SDL_HasIntersection(&AIPlayer->simRect.getRect(), &player1->tail[i]->Rect.getRect()))
			return 1; //Player 1 wins!
	}
	// between p2 and p2 tail
	for (int i = 0; i < AIPlayer->getLiveParticles(); i++)
	{
		if (AIPlayer->tail[i] != nullptr && SDL_HasIntersection(&AIPlayer->simRect.getRect(), &AIPlayer->tail[i]->Rect.getRect()) && AIPlayer->tail[i]->getFrame() > 10)
			return 1; //Player 1 wins!
	}

	//if no collisions
	return 0;
}

int SPCollisionHandler::CheckProjectileCollisions()
{
	for (int i = 0; i < 10; i++)
	{
		if (projectiles[i] != nullptr)
		{
			////check for player kills
			//SDL_Rect tColRect = projectiles[i]->collisionRect.getRect();
			//tColRect.x += 10;
			//tColRect.y += 10;
			//tColRect.w -= 20;
			//tColRect.h -= 20;
			//if (SDL_HasIntersection(&tColRect, &player1->simRect.getRect()))
			//	return 2; // player 2 wins!
			//if (SDL_HasIntersection(&tColRect, &player2->simRect.getRect()))
			//	return 1; //player 1 wins!

			SDL_Rect txRect = projectiles[i]->CollisionX.getRect();
			SDL_Rect tyRect = projectiles[i]->CollisionY.getRect();

			if (SDL_HasIntersection(&txRect, &player1->simRect.getRect()) || SDL_HasIntersection(&tyRect, &player1->simRect.getRect()))
				return 2; // player 2 wins!
			if (SDL_HasIntersection(&txRect, &AIPlayer->simRect.getRect()) || SDL_HasIntersection(&tyRect, &AIPlayer->simRect.getRect()))
				return 1; //player 1 wins!

			//check for projectile on tail collisions and delete portions of the tail
			for (int j = 0; j < player1->getLiveParticles(); j++)
			{
				if (player1->tail[j] != nullptr && SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &player1->tail[j]->Rect.getRect()))
				{
					delete player1->tail[j];
					player1->tail[j] = nullptr;
				}
			}
			for (int j = 0; j < AIPlayer->getLiveParticles(); j++)
			{
				if (AIPlayer->tail[j] != nullptr && SDL_HasIntersection(&projectiles[i]->collisionRect.getRect(), &AIPlayer->tail[j]->Rect.getRect()))
				{
					delete AIPlayer->tail[j];
					AIPlayer->tail[j] = nullptr;
				}
			}
		}
	}
	//if nobody died
	return 0;
}
