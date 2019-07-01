#pragma once
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "macros.h"
#include "Rectangle.h"
#include "TextureBank.h"

#ifndef __MENUBUTTON_H__
#define __MENUBUTTON_H__

class menuButton
{
protected:
	//position and dimensions on the screen
	SDL_Point mPosition;

	SDL_Color DefaultColor;
	SDL_Color HoldColor;
	SDL_Color HoverColor;

	Rectangle hoverBox;

	//Button constants
	int BUTTON_WIDTH = 300;
	int BUTTON_HEIGHT = 100;
	std::string buttonText;

	SDL_Texture* ttfTexture = NULL;
	static Texture* inactiveButtonTexture;
	static Texture* HeldButtonTexture;
	static Texture* activeButtonTexture;
	static Texture* activeBackTexture;


	TTF_Font *Font = NULL;

	SDL_Renderer* Renderer = NULL;

protected:
	//void loadTexture(SDL_Renderer* Renderer);
public:
	int textWidth, textHeight;
	//change to true when clicked
	bool pressed = false;
	bool hovering = false;
	bool held = false;
	Rectangle displayRect;

	SDL_Rect mRect;
	SDL_Rect textRect;

public:
	menuButton();
	//menuButton(int x, int y, std::string text);
	~menuButton();
	menuButton(Rectangle oRect, std::string text, SDL_Renderer* oRenderer);

	virtual void handleEvent(SDL_Event* e);
	bool checkMouse(int x, int y);

	virtual bool render();
	bool render(SDL_Renderer* Renderer);
	bool render(SDL_Renderer* oRenderer, SDL_Color buttonColor);
	bool loadText(SDL_Renderer* Renderer);

	void changeText(std::string text) { buttonText = text; }
	std::string getText() { return buttonText; }
	void setPosition(int x, int y);
	void setDimensions(int w, int h);
	void setDimensions(int w, int h, SDL_Renderer* Renderer);
	void setHover(bool h) { hovering = h; }
	bool getHover() { return hovering; }
	bool setTextColor(SDL_Color tCol);

	void cleanup();
};

//GAMEPAD CONTROLLER BUTTON CLASS
class GPMenuButton : public menuButton
{
public:
	GPMenuButton();
	GPMenuButton(Rectangle oRect, std::string text, SDL_Renderer* r);
	~GPMenuButton();

	void handleEvent(SDL_Event* e);
};

class InfoButton : public menuButton
{
protected:
	static Texture* PanelTexture;
public:
	InfoButton();
	InfoButton(Rectangle oRect, std::string text, SDL_Renderer* r);
	~InfoButton();

	bool render();

};

class BackButton : public GPMenuButton {
public:
	BackButton(Rectangle oRect, std::string text, SDL_Renderer* r);
	bool render();
};
#endif