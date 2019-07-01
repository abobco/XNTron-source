#include "menuButton.h"

Texture* menuButton::inactiveButtonTexture = NULL;
Texture* menuButton::HeldButtonTexture = NULL;
Texture* menuButton::activeBackTexture = NULL;
Texture* menuButton::activeButtonTexture = NULL;

menuButton::menuButton()
{
	setPosition(300, 300);
	mRect.h = BUTTON_HEIGHT;
	mRect.w = BUTTON_WIDTH;
	buttonText = "NEW STAGE";
}
menuButton::menuButton(Rectangle oRect, std::string text, SDL_Renderer* oRenderer)
{
	Renderer = oRenderer;
	mRect = oRect.getRect();
	setPosition(mRect.x, mRect.y);
	setDimensions(mRect.w, mRect.h);
	buttonText = text;

	displayRect = oRect;

	DefaultColor = { 120,120,120,0 };
	HoldColor = { 100,190,100,0 };
	HoverColor = { 240,240,40,0 };

	hoverBox = Rectangle(mPosition.x - 2, mPosition.y - 2, BUTTON_WIDTH + 4, BUTTON_HEIGHT + 4);
	loadText(Renderer);

	textRect.w = mRect.w = oRect.getDimensions().x;
	textRect.h = mRect.h = oRect.getDimensions().y;
}

menuButton::~menuButton()
{
	SDL_DestroyTexture(ttfTexture);
	//delete ttfTexture;
	//ttfTexture = nullptr;
	//delete Font;
	//Font = nullptr;
}
//============================================================================================//
//	setters
//============================================================================================//
void menuButton::setPosition(int x, int y)
{
	mPosition.x = mRect.x = x;
	mPosition.y = mRect.y = y;
}
void menuButton::setDimensions(int w, int h)
{
	mRect.w = BUTTON_WIDTH = w;
	mRect.h = BUTTON_HEIGHT = h;

	displayRect.setDimensions(w, h);
}

void menuButton::setDimensions(int w, int h, SDL_Renderer* Renderer)
{
	mRect.w = BUTTON_WIDTH = w;
	mRect.h = BUTTON_HEIGHT = h;

	displayRect.setDimensions(w, h);
	loadText(Renderer);
}

//============================================================================================//
//	event loop
//============================================================================================//
void menuButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_KEYDOWN)
	{
		//Check collision of mouse and button
			//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = checkMouse(x, y);

		if (inside)
		{
			hovering = true;
			//BUTTON EVENT CODE
			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				displayRect.setColor(100, 190, 100);
				held = true;
				break;
			case SDL_MOUSEBUTTONUP:
				held = false;
				pressed = true;
				break;
			}
		}
		else hovering = false;
	}
}
bool menuButton::checkMouse(int x, int y)
{
	//Check if mouse is in button
	bool inside = true;

	//Mouse is left of the button
	if (x < mPosition.x)
	{
		held = inside = false;
	}
	//Mouse is right of the button
	else if (x > mPosition.x + displayRect.getDimensions().x)
	{
		held = inside = false;
	}
	//Mouse above the button
	else if (y < mPosition.y)
	{
		held = inside = false;
	}
	//Mouse below the button
	else if (y > mPosition.y + displayRect.getDimensions().y)
	{
		held = inside = false;
	}
	return inside;
}


//============================================================================================//
//	rendering
//============================================================================================//
bool menuButton::render(SDL_Renderer* Renderer)
{
	bool success = true;

	static int activeButtonAnimator = 0;
	static bool forward = true;
	SDL_Point pos = displayRect.getPosition();
	SDL_Point dimensions = displayRect.getDimensions();
	SDL_Rect oRect = { 0,0,dimensions.x,dimensions.y };
	SDL_Rect oTextRect = { 0,0,textRect.w,textRect.h };
	SDL_Rect oHoverRect = { 0,0,dimensions.x + 4, dimensions.y + 4 };
	SDL_Rect screen = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_RenderSetViewport(Renderer, &hoverBox.getRect());
	if (hovering)
	{
		SDL_SetRenderDrawColor(Renderer, HoverColor.r, HoverColor.g, HoverColor.b, HoverColor.a);
		SDL_RenderFillRect(Renderer, &oHoverRect);

		SDL_RenderSetViewport(Renderer, &screen);

		if (activeButtonAnimator < 20 && activeButtonAnimator > 0)
		{
			if (forward)
				activeButtonAnimator++;
			else
				activeButtonAnimator--;
		}
		else
		{
			if (activeButtonAnimator >= 20)	{
				--activeButtonAnimator;
				forward = false;
			}
			else {
				++activeButtonAnimator;
				forward = true;
			}
		}

		activeButtonTexture->Render(pos.x - activeButtonTexture->GetWidth() - activeButtonAnimator, pos.y);
	}
	SDL_RenderSetViewport(Renderer, &displayRect.getRect());
	if (held)
	{
		SDL_Color tempcol = displayRect.getColor();
		SDL_SetRenderDrawColor(Renderer, tempcol.r, tempcol.b, tempcol.g, 0xFF);
	}
	else
		SDL_SetRenderDrawColor(Renderer, 0x8F, 0x99, 0xAA, 0xFF);

	SDL_RenderFillRect(Renderer, &oRect);
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(Renderer, &oRect);

	SDL_RenderCopyEx(Renderer, ttfTexture, 0, &oTextRect, 0, 0, SDL_FLIP_NONE);

	SDL_RenderSetViewport(Renderer, &screen);
	return success;
}

bool menuButton::render()
{
	bool success = true;

	static int activeButtonAnimator = 0;
	static bool forward = true;

	SDL_Point pos = displayRect.getPosition();
	SDL_Point dimensions = displayRect.getDimensions();
	SDL_Rect oRect = { 0,0,dimensions.x,dimensions.y };
	SDL_Rect oTextRect = { 0,0,textRect.w,textRect.h };
	SDL_Rect oHoverRect = { 0,0,dimensions.x + 4, dimensions.y + 4 };
	SDL_Rect screen = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_RenderSetViewport(Renderer, &hoverBox.getRect());
	if (hovering)
	{
		//SDL_SetRenderDrawColor(Renderer, HoverColor.r, HoverColor.g, HoverColor.b, HoverColor.a);
		//SDL_RenderFillRect(Renderer, &oHoverRect);

		SDL_RenderSetViewport(Renderer, &screen);

		if (activeButtonAnimator < 50 && activeButtonAnimator > 0)
		{
			if (forward)
				activeButtonAnimator++;
			else
				activeButtonAnimator--;
		}
		else
		{
			if (activeButtonAnimator >= 50) {
				--activeButtonAnimator;
				forward = false;
			}
			else {
				++activeButtonAnimator;
				forward = true;
			}
		}

		activeButtonTexture->Render(pos.x - activeButtonTexture->GetWidth() - activeButtonAnimator, pos.y, activeButtonTexture->GetWidth(), dimensions.y);
	}
	SDL_RenderSetViewport(Renderer, &displayRect.getRect());
	if (held)
	{
		SDL_Color tempcol = displayRect.getColor();
		SDL_SetRenderDrawColor(Renderer, tempcol.r, tempcol.b, tempcol.g, 0xFF);
		if (HeldButtonTexture != NULL)
			HeldButtonTexture->Render(0, 0, oRect.w, oRect.h);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer, 57, 132, 14, 0);
		if (inactiveButtonTexture != NULL)
			inactiveButtonTexture->Render(0, 0, oRect.w, oRect.h);
	}

	//SDL_RenderFillRect(Renderer, &oRect);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
	//SDL_RenderDrawRect(Renderer, &oRect);

	SDL_RenderCopyEx(Renderer, ttfTexture, 0, &oTextRect, 0, 0, SDL_FLIP_NONE);

	SDL_RenderSetViewport(Renderer, &screen);
	return success;
}

//============================================================================================//
//	font loading
//============================================================================================//
bool menuButton::loadText(SDL_Renderer* Renderer)
{
	bool success = true;
	//Init Font
	if (TTF_Init() < 0) {
		// Error handling code
		printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	}
	Font = TTF_OpenFont("zero hour.ttf", 72);
	if (Font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	SDL_Color fontcolor;
	fontcolor.a = 0;
	fontcolor.r= 57;
	fontcolor.g= 14;
	fontcolor.b= 132;

	std::string txt = buttonText;
	SDL_Surface* surface = TTF_RenderText_Solid(Font, txt.c_str(), fontcolor);
	if (surface == NULL)
	{
		printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	ttfTexture = SDL_CreateTextureFromSurface(Renderer, surface);


	SDL_QueryTexture(ttfTexture, NULL, NULL, &textWidth, &textHeight);
	textRect.x = displayRect.getPosition().x;
	textRect.y = displayRect.getPosition().y;
	textRect.w = textWidth;
	textRect.h = displayRect.getDimensions().y;

	mRect.w = textWidth;


	SDL_FreeSurface(surface);

	return success;
}

bool menuButton::setTextColor(SDL_Color tCol)
{
	bool success = true;
	//Init Font
	if (TTF_Init() < 0) {
		// Error handling code
		printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
		success = false;
	}
	Font = TTF_OpenFont("zero hour.ttf", 72);
	if (Font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	std::string txt = buttonText;
	SDL_Surface* surface = TTF_RenderText_Solid(Font, txt.c_str(), tCol);
	if (surface == NULL)
	{
		printf("Failed to create surface! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	ttfTexture = SDL_CreateTextureFromSurface(Renderer, surface);


	SDL_QueryTexture(ttfTexture, NULL, NULL, &textWidth, &textHeight);
	textRect.x = displayRect.getPosition().x;
	textRect.y = displayRect.getPosition().y;
	textRect.w = textWidth;
	textRect.h = displayRect.getDimensions().y;

	mRect.w = textWidth;


	SDL_FreeSurface(surface);

	textRect.w = mRect.w = displayRect.getDimensions().x;
	textRect.h = mRect.h = displayRect.getDimensions().y;

	textRect.w -= 50;

	return success;
}

void menuButton::cleanup()
{
	SDL_DestroyTexture(ttfTexture);
	ttfTexture = NULL;
}


//GAMEPAD CONTROLLER BUTTON CLASS
GPMenuButton::GPMenuButton()
{
	setPosition(300, 300);
	mRect.h = BUTTON_HEIGHT;
	mRect.w = BUTTON_WIDTH;
	buttonText = "BUTTON TEXT";
}

GPMenuButton::GPMenuButton(Rectangle oRect, std::string text, SDL_Renderer * r)
{
	Renderer = r;
	mRect = oRect.getRect();
	setPosition(mRect.x, mRect.y);
	setDimensions(mRect.w, mRect.h);
	buttonText = text;
	displayRect = oRect;

	inactiveButtonTexture = TextureBank::Get("red_button01");
	HeldButtonTexture = TextureBank::Get("red_button02");
	activeButtonTexture = TextureBank::Get("red_sliderRight");
	activeBackTexture = TextureBank::Get("red_sliderLeft");

	DefaultColor = { 120,120,120,0 };
	HoldColor = { 100,190,100,0 };
	HoverColor = { 240,240,40,0 };

	hoverBox = Rectangle(mPosition.x - 2, mPosition.y - 2, BUTTON_WIDTH + 4, BUTTON_HEIGHT + 4);
	loadText(Renderer);

	textRect.w = mRect.w = oRect.getDimensions().x;
	textRect.h = mRect.h = oRect.getDimensions().y;
}

GPMenuButton::~GPMenuButton()
{
	SDL_DestroyTexture(ttfTexture);
}

//the menu class will deal with deciding which controller button is hovering
void GPMenuButton::handleEvent(SDL_Event * e)
{
	if (hovering)
	{
		if (e->type == SDL_CONTROLLERBUTTONDOWN && e->cbutton.button == SDL_CONTROLLER_BUTTON_A)
		{
			held = true;
		}
		if (e->type == SDL_CONTROLLERBUTTONUP && e->cbutton.button == SDL_CONTROLLER_BUTTON_A)
		{
			held = false;
			pressed = true;
		}
	}
}

Texture* InfoButton::PanelTexture = NULL;

InfoButton::InfoButton()
{
	setPosition(300, 300);
	mRect.h = BUTTON_HEIGHT;
	mRect.w = BUTTON_WIDTH;
	buttonText = "BUTTON TEXT";
}

InfoButton::InfoButton(Rectangle oRect, std::string text, SDL_Renderer * r)
{
	Renderer = r;
	mRect = oRect.getRect();
	setPosition(mRect.x, mRect.y);
	setDimensions(mRect.w, mRect.h);
	buttonText = text;
	displayRect = oRect;

	PanelTexture = TextureBank::Get("red_panel");

	DefaultColor = { 120,120,120,0 };
	HoldColor = { 100,190,100,0 };
	HoverColor = { 240,240,40,0 };

	hoverBox = Rectangle(mPosition.x - 2, mPosition.y - 2, BUTTON_WIDTH + 4, BUTTON_HEIGHT + 4);
	loadText(Renderer);

	textRect.w = mRect.w = oRect.getDimensions().x;
	textRect.h = mRect.h = oRect.getDimensions().y;

	textRect.w -= 50;
}


InfoButton::~InfoButton()
{
	SDL_DestroyTexture(ttfTexture);
}

bool InfoButton::render()
{
	SDL_Point dimensions = displayRect.getDimensions();
	SDL_Rect oRect = { 0,0,dimensions.x,dimensions.y };
	SDL_Rect oTextRect = { 25,0,textRect.w,textRect.h };
	SDL_Rect screen = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_RenderSetViewport(Renderer, &displayRect.getRect());
	if (PanelTexture != NULL)
		PanelTexture->Render(0, 0, oRect.w, oRect.h);
	else
		return false;


	//SDL_RenderFillRect(Renderer, &oRect);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
	//SDL_RenderDrawRect(Renderer, &oRect);

	SDL_RenderCopyEx(Renderer, ttfTexture, 0, &oTextRect, 0, 0, SDL_FLIP_NONE);

	SDL_RenderSetViewport(Renderer, &screen);
	return true;
}



BackButton::BackButton(Rectangle oRect, std::string text, SDL_Renderer * r)
{
	Renderer = r;
	mRect = oRect.getRect();
	setPosition(mRect.x, mRect.y);
	setDimensions(mRect.w, mRect.h);
	buttonText = text;
	displayRect = oRect;

	inactiveButtonTexture = TextureBank::Get("red_button01");
	HeldButtonTexture = TextureBank::Get("red_button02");
	activeButtonTexture = TextureBank::Get("red_sliderRight");
	activeBackTexture = TextureBank::Get("red_sliderLeft");

	DefaultColor = { 120,120,120,0 };
	HoldColor = { 100,190,100,0 };
	HoverColor = { 240,240,40,0 };

	hoverBox = Rectangle(mPosition.x - 2, mPosition.y - 2, BUTTON_WIDTH + 4, BUTTON_HEIGHT + 4);
	loadText(Renderer);

	textRect.w = mRect.w = oRect.getDimensions().x;
	textRect.h = mRect.h = oRect.getDimensions().y;
}

bool BackButton::render()
{
	bool success = true;

	static int backButtonAnimator = 0;
	static bool forward = true;

	SDL_Point pos = displayRect.getPosition();
	SDL_Point dimensions = displayRect.getDimensions();
	SDL_Rect oRect = { 0,0,dimensions.x,dimensions.y };
	SDL_Rect oTextRect = { 0,0,textRect.w,textRect.h };
	SDL_Rect oHoverRect = { 0,0,dimensions.x + 4, dimensions.y + 4 };
	SDL_Rect screen = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	SDL_RenderSetViewport(Renderer, &hoverBox.getRect());
	if (hovering)
	{
		//SDL_SetRenderDrawColor(Renderer, HoverColor.r, HoverColor.g, HoverColor.b, HoverColor.a);
		//SDL_RenderFillRect(Renderer, &oHoverRect);

		SDL_RenderSetViewport(Renderer, &screen);

		if (backButtonAnimator < 50 && backButtonAnimator > 0)
		{
			if (forward)
				backButtonAnimator++;
			else
				backButtonAnimator--;
		}
		else
		{
			if (backButtonAnimator >= 50) {
				--backButtonAnimator;
				forward = false;
			}
			else {
				++backButtonAnimator;
				forward = true;
			}
		}

		activeBackTexture->Render(pos.x + dimensions.x + backButtonAnimator, pos.y, activeBackTexture->GetWidth(), dimensions.y);
	}
	SDL_RenderSetViewport(Renderer, &displayRect.getRect());
	if (held)
	{
		SDL_Color tempcol = displayRect.getColor();
		SDL_SetRenderDrawColor(Renderer, tempcol.r, tempcol.b, tempcol.g, 0xFF);
		if (HeldButtonTexture != NULL)
			HeldButtonTexture->Render(0, 0, oRect.w, oRect.h);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer, 57, 132, 14, 0);
		if (inactiveButtonTexture != NULL)
			inactiveButtonTexture->Render(0, 0, oRect.w, oRect.h);
	}

	//SDL_RenderFillRect(Renderer, &oRect);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
	//SDL_RenderDrawRect(Renderer, &oRect);

	SDL_RenderCopyEx(Renderer, ttfTexture, 0, &oTextRect, 0, 0, SDL_FLIP_NONE);

	SDL_RenderSetViewport(Renderer, &screen);
	return success;
}
