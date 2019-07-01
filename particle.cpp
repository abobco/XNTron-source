#include "particle.h"
int Particle::maxframe = 0;

Particle::Particle()
{
	xPos = 0;
	yPos = 0;
	frame = 0;
	maxframe = 0;
	Rect = Rectangle(xPos, yPos, PARTICLE_WIDTH, PARTICLE_HEIGHT, 100, 0, 200);
}


//Checks if particle is dead
bool Particle::isDead()
{
	return frame > maxframe;
}

void Particle::setPosition(int x, int y)
{
	xPos = x;
	yPos = y;
	Rect.setPosition(x, y);
	rRect.setPosition(x, y);
}

void Particle::setYPosition(int y)
{
	yPos = y;
	Rect.setPosition(xPos, y);
	rRect.setPosition(xPos, y);
}


PermanentParticle::PermanentParticle(int x, int y, SDL_Renderer* oRenderer, int player)
{
	xPos = x;
	yPos = y;
	Renderer = oRenderer;
	maxframe = 0;
	Rect.setPosition(xPos, yPos);
	if (player == 1)
		Rect.setColor(168, 0, 0);
	else
		Rect.setColor(0, 0, 168);
}

PermanentParticle::PermanentParticle(int x, int y, SDL_Renderer * r, SDL_Color c)
{
	xPos = x;
	yPos = y;
	Renderer = r;
	maxframe = 0;
	Rect.setPosition(xPos, yPos);
	Rect.setColor(c.r, c.g, c.b);
}

void PermanentParticle::render()
{
	Rect.render(Renderer);
	frame++;
}