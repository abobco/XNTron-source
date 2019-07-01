#pragma once

#include <SDL.h>
#include "macros.h"
#include <math.h>
#include "Rectangle.h"
#include "particle.h"
#include "TextureBank.h"

#ifndef __COMPETITOR_H__
#define __COMPETITOR_H__

class Competitor
{
protected:
	int xPos, yPos, xVel, yVel;
	int player = 1;
	int ammo = 5;

	double angle = 0;
	double oldAngle = 0;

	const int STUPIDITY_INDEX = 30;	//range of restricted input angles

	static const int MAX_PARTICLES = 20000;
	int liveParticles = 0;
	char ship = 'r';

	SDL_Renderer* Renderer = NULL;
	SDL_Rect windowRect = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_Color col;
	SDL_Point center;
	SDL_Point oldVel = { 0,0 };

	static Texture* ShipTexture;
	static Texture* ShipTextureRED;

public:
	static const int HEAD_WIDTH = 36, HEAD_HEIGHT = 36;
	static const int MAX_VEL = 5;
	Rectangle TextureRect;
	Rectangle simRect;
	PermanentParticle* tail[MAX_PARTICLES];
	bool loser = false;
	bool updateAmmo = false;

protected:
	void renderParticles();
	void checkCollisions();

public:
	Competitor();
	Competitor(SDL_Renderer* oRenderer);
	~Competitor();

	void handleEvent(SDL_Event* e);
	void move();
	void render();

	int getLiveParticles() { return liveParticles; }
	SDL_Point getPosition() { return SDL_Point{ xPos,yPos }; }
	SDL_Point getVelocity() { return SDL_Point{ xVel,yVel }; }
	void setAngle(double a) { angle = a; }
	double getAngle() { return angle; }
	void setPosition(int x, int y);
	void setVelAngle(double angle); 	//in degrees
	SDL_Color getColor() { return col; }
	void setColor(SDL_Color c) { col = c; simRect.setColor(c.r, c.g, c.b); }
	void setShip(char c) { ship = c; }
};

class ControllerCompetitor : public Competitor
{
private:
	SDL_Joystick* joy;

private:
	void checkSuicide();
public:
	ControllerCompetitor();
	ControllerCompetitor(SDL_Renderer* oRenderer, SDL_Joystick* j);
	~ControllerCompetitor();
	void handleEvent(SDL_Event* e, SDL_Joystick* j);
	
	void handleEvent(SDL_Event* e);
};

class AICompetitor : public Competitor {
private:
	int frames = 0;
	int shoot = 0;
public:
	AICompetitor();
	AICompetitor(SDL_Renderer* r);
	~AICompetitor();
	void move();

	int getShoot() { return shoot; }
	void setShoot(int s) { shoot = s; }
};
#endif // !__COMPETITOR_H__
