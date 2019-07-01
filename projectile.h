#pragma once

#include <SDL.h>
#include "Rectangle.h"
#include "macros.h"
#include <math.h>
#include "TextureBank.h"

#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

class Projectile {
private:
	int xPos, yPos, xVel, yVel;
	static const int SIZE = 50;
	static const int MAX_VEL = 10;
	double angle;
	//SDL_Color color;
	SDL_Renderer* Renderer = NULL;

	char color = 'r';

	static Texture* BlueTexture;
	static Texture* RedTexture;

public:
	Rectangle collisionRect;
	Rectangle CollisionX;
	Rectangle CollisionY;

public:
	Projectile();
	Projectile(SDL_Renderer* oRenderer, int oxPos, int oyPos, int oxVel, int oyVel, char col);
	~Projectile();

	void move();
	void render();
};

#endif // !__PROJECTILE_H__
