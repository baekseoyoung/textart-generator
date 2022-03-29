#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

class MyStyle
{
	public:
		int winW = 700;
		int winH = 500;
		void func_window_update(SDL_Window *window) {
			SDL_GetWindowSize(window, &winW, &winH);
		}

		// {x, y, w, h}
		int editc[4] = {0, 0, winW-200, winH-200}; //{0, 0, winW-200, winH-200};
		int sidec[4] = {winW-200, 0, 200, winH}; //{winW-200, 0, winW, winH};
		int downc[4] = {0, winH-200, winW-200, 200}; //{0, winH-200, winW-200, winH};

		string fontPath = "assets/unifont-14.0.02.ttf";
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 20;
		int txtW = 64;
		int txtH = 64;
		int downW = 300;
		int downH = 100;
		int border = 5;

		SDL_Color cursorColor = {0,255,0,0xFF};
		SDL_Rect cursorRect = {0,0,0,0};
};