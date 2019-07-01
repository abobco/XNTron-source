#include "projectile.h"

Texture* Projectile::BlueTexture = NULL;
Texture* Projectile::RedTexture = NULL;

Projectile::Projectile()
{
	xPos = yPos = xVel = yVel = 0;
	angle = 0;
	collisionRect = Rectangle(xPos, yPos, SIZE, SIZE,80,220,100);

	CollisionY = Rectangle(xPos + 22, yPos + 8, 3, 39);
	CollisionX = Rectangle(xPos , yPos + 22, 39, 3);
}

Projectile::Projectile(SDL_Renderer * oRenderer, int oxPos, int oyPos, int oxVel, int oyVel, char col)
{
	Renderer = oRenderer;
	xPos = oxPos;
	yPos = oyPos;
	angle = 0;

	xVel = oxVel * 3.0/2;
	yVel = oyVel * 3.0/2;

	color = col;

	collisionRect = Rectangle(xPos, yPos, SIZE, SIZE, 80, 220, 100);

	CollisionY = Rectangle(xPos + 22, yPos + 8, 3, 39);
	CollisionX = Rectangle(xPos , yPos + 22, 39, 3);

	BlueTexture = TextureBank::Get("ProjectileBLUE");
	RedTexture = TextureBank::Get("ProjectileRED");
}

Projectile::~Projectile()
{
}

void Projectile::move()
{
	xPos += xVel;
	yPos += yVel;

	collisionRect.setPosition(xPos, yPos);
	CollisionX.setPosition(xPos + 4, yPos + 22);
	CollisionY.setPosition(xPos + 22, yPos + 4);
}

void Projectile::render()
{
	//collisionRect.render(Renderer);
	switch (color) {
	case 'r':
		RedTexture->Render(xPos, yPos);
		break;
	case'b':
		BlueTexture->Render(xPos, yPos);
		break;
	default:
		break;
	}
}
