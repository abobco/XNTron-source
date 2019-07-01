#pragma once
#include <SDL.h>
#include "Competitor.h"
#include "projectile.h"
#include "particle.h"
#include "macros.h"

#ifndef __COLLISIONHANDLER_H__
#define __COLLISIONHANDLER_H__

class CollisionHandler
{
protected:

	int shotProjectiles = 0;
	SDL_Rect killBox = { 24,124, WINDOW_WIDTH - 48, WINDOW_HEIGHT - 148 };
public:
	Projectile* projectiles[10];
	ControllerCompetitor* player1;
	ControllerCompetitor* player2;
protected:
	bool CheckAngledCollision(Rectangle A, Rectangle B);
public:
	CollisionHandler();
	CollisionHandler(ControllerCompetitor* p1, ControllerCompetitor* p2);
	~CollisionHandler();

	int CheckEdgeCollisions();
	int CheckPlayerCollisions();
	int CheckProjectileCollisions();

	void addProjectile(Projectile* p);

	void moveProjectiles();
};

class SPCollisionHandler : public CollisionHandler {
public:
	AICompetitor* AIPlayer;
public:
	SPCollisionHandler();
	SPCollisionHandler(ControllerCompetitor* p1, AICompetitor* AIp);
	~SPCollisionHandler();

	void addProjectile(Projectile* p);

	int CheckEdgeCollisions();
	int CheckPlayerCollisions();
	int CheckProjectileCollisions();
};
#endif
