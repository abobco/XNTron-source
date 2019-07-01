#pragma once
#include "Competitor.h"
#include <iostream>

Texture* Competitor::ShipTexture = NULL;
Texture* Competitor::ShipTextureRED = NULL;

//first player
Competitor::Competitor()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		tail[i] = nullptr;
	}
}
Competitor::Competitor(SDL_Renderer * oRenderer)
{
	//Initialize the offsets
	xPos = 100;
	yPos = 100;
	//Initialize the velocity
	xVel = MAX_VEL-1;
	yVel = 0;

	Renderer = oRenderer;

	TextureRect = Rectangle(xPos, yPos, HEAD_WIDTH, HEAD_HEIGHT, 100, 0, 200);
	simRect = Rectangle(xPos, yPos, 20, 20,168,0,0);

	ShipTexture = TextureBank::Get("RotatingShip");

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
			tail[i] = nullptr;
	}
}
Competitor::~Competitor()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if ( tail != nullptr)
		delete tail[i];
		tail[i] = nullptr;
	}
}

void Competitor::setPosition(int x, int y)
{
	xPos = x;
	yPos = y;
	TextureRect.setPosition(x, y);
	simRect.setPosition(x, y);
}

void Competitor::renderParticles()
{
	for (int i = 0; i < liveParticles; i++)
	{
		if (tail[i] != nullptr)
		{
			tail[i]->render();
		}
	}
}

void Competitor::checkCollisions()
{
	if (xPos < 0)
		xPos = 0;
	else if (xPos > WINDOW_WIDTH)
		xPos = WINDOW_WIDTH - 20;
	if (yPos < 0)
		yPos = 0;
	else if (yPos > WINDOW_HEIGHT)
		yPos = WINDOW_HEIGHT - 20;
}
void Competitor::render()
{
	renderParticles();
	SDL_Point tPos = TextureRect.getPosition();
	switch (ship)
	{
	case 'b':
		ShipTexture->Render(tPos.x, tPos.y, angle);
		break;
	case 'r':
		ShipTextureRED->Render(tPos.x, tPos.y, angle);
		break;
	default:
		break;
	}
	//simRect.render(Renderer);
}
void Competitor::setVelAngle(double angle)
{
	xVel = MAX_VEL * cos(angle*M_PI / 180);
	yVel = MAX_VEL * sin(angle*M_PI / 180);
}
//void Competitor::passCollisionEngine(CollisionHandler& c)
//{
//	projectileOutput = &c;
//}
//void Competitor::shootProjectile()
//{
//	if (projectileOutput!= nullptr)
//		projectileOutput->addProjectile(new Projectile(Renderer, xPos, yPos, angle));
//}
void Competitor::move()
{

	//Move the point left or right
	xPos += xVel;
	//Move the point up or down
	yPos += yVel;
	//checkCollisions();

	if (liveParticles < MAX_PARTICLES)
	{
		tail[liveParticles] = new PermanentParticle(xPos + 5, yPos + 5, Renderer, col);
		liveParticles++;
	}
	
	TextureRect.setPosition(xPos-10, yPos- 10);
	simRect.setPosition(xPos, yPos);

	
}

void Competitor::handleEvent(SDL_Event * e)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_Point mousePos{ x, y };

	/*if (SDL_PointInRect(&mousePos, &Rect.getRect()))
		xVel = yVel = 0;
	else
	{*/
	int deltaX = x - xPos, deltaY = yPos - y;

	angle = atan(double(deltaY) / deltaX);


	if (deltaY * deltaX < 0)
		angle *= -1;

	if (x > xPos) {
		xVel = MAX_VEL * cos(angle);
	}
	else {
		xVel = -MAX_VEL * cos(angle);
	}
	if (y > yPos) {
		yVel = MAX_VEL * sin(angle);
	}
	else {
		yVel = -MAX_VEL * sin(angle);
	}

	//if (e->type == SDL_MOUSEBUTTONDOWN && ammo > 0 && projectileOutput != nullptr)
	//	shootProjectile();
		
}



//2nd player
ControllerCompetitor::ControllerCompetitor()
{
}
ControllerCompetitor::ControllerCompetitor(SDL_Renderer * oRenderer, SDL_Joystick* j)
{
	joy = j;
	//Initialize the offsets
	xPos = WINDOW_WIDTH -100;
	yPos = WINDOW_HEIGHT -100;
	//Initialize the velocity
	xVel = (-MAX_VEL)+1;
	yVel = 0;

	Renderer = oRenderer;

	TextureRect = Rectangle(xPos, yPos, HEAD_WIDTH, HEAD_HEIGHT, 100, 0, 200);
	simRect = Rectangle(xPos, yPos, 20, 20, 0,0,168);

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		tail[i] = nullptr;
	}
	player = 2;

	ShipTexture = TextureBank::Get("RotatingShip");
	ShipTextureRED = TextureBank::Get("RotatingShipRED");
}
ControllerCompetitor::~ControllerCompetitor()
{
}

void ControllerCompetitor::handleEvent(SDL_Event * e, SDL_Joystick* j)
{
		int jvalX = SDL_JoystickGetAxis(j, 0);
		int jvalY = SDL_JoystickGetAxis(j, 1);

		angle = atan(double(jvalY) / jvalX);


		if (jvalX * jvalY < 0)
			angle *= -1;


		if (jvalX >0)
			xVel = MAX_VEL * cos(angle);
		else
			xVel = -MAX_VEL * cos(angle);
		if (jvalY >0)
			yVel = MAX_VEL * sin(angle);
		else
			yVel = -MAX_VEL * sin(angle);
}

void ControllerCompetitor::handleEvent(SDL_Event * e)
{
	int jvalX = SDL_JoystickGetAxis(joy, 0);
	int jvalY = SDL_JoystickGetAxis(joy, 1);

	oldAngle = angle;
	oldVel = { xVel, yVel };

	if (abs(jvalX) > 5000 || abs(jvalY) > 5000)
	{
		angle = atan(double(jvalY) / jvalX);


		if (jvalX * jvalY < 0)
			angle *= -1;


		if (jvalX > 0) {
			xVel = MAX_VEL * cos(angle);
		}
		else {
			xVel = -MAX_VEL * cos(angle);
		}
		if (jvalY > 0) {
			yVel = MAX_VEL * sin(angle);
			// change the angle again for rendering, SDL rotates clockwise w/ positive angle
			angle *= 180 / M_PI; // convert to radians
			if (angle < 0)
				angle *= -1;	// convert to positive angle to TRY to make it less confusing
			if (jvalX > 0) { //Quad 4
				angle = 360 - angle;	
				angle *= -1;
				angle += 360;
				//std::cout << 4 << ' ';
			}
			else {			//Quad 3
				angle *= -1;
				angle += 180;
				//std::cout << 3 << ' ';
			}
		}
		else {	// if jvalY < 0
			yVel = -MAX_VEL * sin(angle);
			angle *= 180 / M_PI;	//same shit
			if (angle < 0)
				angle *= -1;
			if (jvalX > 0) {	//Quad 1
				angle *= -1;
				angle += 360;
				//std::cout << 1 << ' ';
			}
			else {				//Quad 2
				angle = 180 - angle;
				angle *= -1;
				angle += 360;
				//std::cout << 2 << ' ';
			}
		}
	}
	checkSuicide();
}
void ControllerCompetitor::checkSuicide()	//prevent input that would instantly kill you from performing a 180 degree turn(or close to 180 degrees)
{
	double oppositeAngle = oldAngle - 180;
	if (oppositeAngle < 0) {
		oppositeAngle += 360;
	}
	//determine angle range of unnacceptable inputs
	int topBounds = oppositeAngle + STUPIDITY_INDEX;
	int botBounds = oppositeAngle - STUPIDITY_INDEX;

	//correct bounds
	if (topBounds > 360) {
		topBounds -= 360;
	}
		if (botBounds < 0) {
			botBounds += 360;
		}

	//actual logic check
	if ((angle < topBounds && angle >botBounds) || (angle == 0 && oldAngle == 180) || (angle == 180 && oldAngle == 0)) {
		angle = oldAngle;
		xVel = oldVel.x;
		yVel = oldVel.y;
	}
}

AICompetitor::AICompetitor()
{
}

AICompetitor::AICompetitor(SDL_Renderer * r)
{
	//Initialize the offsets
	xPos = WINDOW_WIDTH - 100;
	yPos = WINDOW_HEIGHT - 100;
	//Initialize the velocity
	xVel = (-MAX_VEL) + 1;
	yVel = 0;
	angle = 180;

	Renderer = r;

	TextureRect = Rectangle(xPos, yPos, HEAD_WIDTH, HEAD_HEIGHT, 100, 0, 200);
	simRect = Rectangle(xPos, yPos, 20, 20, 0, 0, 168);

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		tail[i] = nullptr;
	}
	player = 2;

	ShipTexture = TextureBank::Get("RotatingShip");
	ShipTextureRED = TextureBank::Get("RotatingShipRED");
}

AICompetitor::~AICompetitor()
{
}

void AICompetitor::move()
{
	
	++frames;
	if (frames == 400) {
		xVel = 0;
		yVel = -MAX_VEL + 1;
		shoot = 1;
		angle = 90;
	}
	if (frames == 410) {
		shoot = 1;
	}
	if (frames ==420) {
		xVel = MAX_VEL - 1;
		yVel = 0;
		angle = 0;
		shoot = 1;
	}
	if (frames == 820) {
		xVel = 0;
		yVel = MAX_VEL - 1;
		angle = 90;
		shoot = 2;
	}
	if (frames == 830)
	{
		shoot = 2;
	}
	if (frames == 840) {
		xVel = -MAX_VEL + 1;
		yVel = 0;
		angle = 180;
		shoot = 2;
		frames = 0;
	}
	//if (xPos > WINDOW_WIDTH - 100) {
	//	xVel = 0;
	//	yVel = MAX_VEL-1;
	//}
	//else if (xPos < 100) {
	//	xVel = 0;
	//	yVel = -MAX_VEL+1;
	//}
	//if (yPos > WINDOW_HEIGHT - 100) {
	//	yVel = 0;
	//	xVel = MAX_VEL - 1;
	//}
	//else if (yPos < 200) {
	//	yVel = 0;
	//	xVel = -MAX_VEL + 1;
	//}



	//Move the point left or right
	xPos += xVel;
	//Move the point up or down
	yPos += yVel;
	//checkCollisions();
	if (liveParticles < MAX_PARTICLES)
	{
		tail[liveParticles] = new PermanentParticle(xPos + 5, yPos + 5, Renderer, col);
		liveParticles++;
	}


	TextureRect.setPosition(xPos - 10, yPos - 10);
	simRect.setPosition(xPos, yPos);
}
