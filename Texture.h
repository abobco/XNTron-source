//==============================================================================
/*
	Texture class for wrapping SDL Textures

*/
//==============================================================================
#ifndef __TEXTURE_H__
	#define __TEXTURE_H__

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class Texture {
	private:
		std::string Filename;

		int Width = 0;
		int Height = 0;

		SDL_Renderer* Renderer = NULL;
		SDL_Texture* SDLTexture = NULL;

	public:
		Texture();
		~Texture();

		bool Load(SDL_Renderer* Renderer, std::string Filename);

		void Render(int X, int Y);

		void Render(int X, int Y, double angle);

		void Render(int X, int Y, int Width, int Height);

		void Render(int X, int Y, int Width, int Height, int SX, int SY, int SWidth, int SHeight);

		int GetWidth();
		int GetHeight();
};

#endif
