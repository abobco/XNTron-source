#pragma once
#include "Rectangle.h"
#include <SDL.h>

#ifndef __PARTICLE_H__
#define __PARTICLE_H__
class Particle
{
protected:
	//Offsets
	int xPos, yPos;
	static const int  PARTICLE_WIDTH = 10, PARTICLE_HEIGHT = 10;
	SDL_Renderer* Renderer;
	 int frame = 0;
	 int angle = 0;
	 SDL_RendererFlip flip = SDL_FLIP_NONE;
public:
	//xollision rectangle
	Rectangle Rect;
	//rendering retangle
	Rectangle rRect;
	static int maxframe;
public:
	//Initialize position and animation
	Particle();
	//Particle(int oframe, int omaxframe, int x, int y, SDL_Renderer* oRenderer);
	//Particle(int oframe, int omaxframe, int x, int y, double angle, int oxVel, int oyVel, SDL_Renderer* oRenderer, SDL_RendererFlip f);

	//Shows the particle
	virtual void render() = 0;

	//Checks if particle is dead
	bool isDead();

	int getFrame() { return frame; }
	void setFrame(int oframe) { frame = oframe; }
	void incFrame() { frame++; }

	static void setMaxFrame(int omaxframe) { maxframe = omaxframe; }

	void setPosition(int x, int y);
	void setYPosition(int y);
};

class PermanentParticle : public Particle
{
public:
	PermanentParticle(int x, int y, SDL_Renderer* oRenderer, int player);
	PermanentParticle(int x, int y, SDL_Renderer* r, SDL_Color c);
	bool isDead() { return false; }
	void render();
};
#endif
