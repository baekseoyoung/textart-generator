#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class SubBox
{
	public:
		//SDL_Surface *outerSurf;
		SDL_Surface *surf; // content
		SDL_Rect rect = {0,0,0,0}; // content
		SDL_Rect box = {0,0,0,0}; // colored border
		SDL_Rect outer = {0,0,0,0};
		SDL_Rect inner = {0,0,0,0};
		int border = 5;
		void setBoxSize(int *arr) {
			int x = arr[0];
			int y = arr[1];
			int w = arr[2];
			int h = arr[3];
			rect.x = 0, rect.y = 0, rect.w = w-4*border, rect.h = h-4*border;
			box.x = x, box.y = y, box.w = w, box.h = h;
			outer.x = x+border, outer.y = y+border, outer.w = w-2*border, outer.h = h-2*border;
			inner.x = x+2*border, inner.y = y+2*border, inner.w = w-4*border, inner.h = h-4*border;
		}
		void showBox(SDL_Surface *screen, Uint32 pixel) {
			SDL_FillRect(screen, &box, pixel);
			SDL_FillRect(screen, &outer, SDL_MapRGBA(screen->format,0,0,0,255));
		}
		~SubBox() {}
};

class EditBox : public SubBox
{
	public:
		//=== font
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 20;

		//=== cursor
		SDL_Surface *curSurf;
		SDL_Rect curRect = {0, 0, 0, 0};
		SDL_Color curColor = {0,255,0,0xFF};
		int curRow = 0;
		int curCol = 0;

		void showSurf(int winW, int winH, vector<string> text, TTF_Font *font, bool timeFlag) {
			surf = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
			SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format,0,50,0,255));
			SDL_Surface *tempSurf;
			SDL_Rect tempRect = {0,0,0,0};
				//=== text display
				for(int i=0; i<(int)text.size(); i++) {
					string line = text[i];
					tempSurf = TTF_RenderUTF8_Blended(font, line.c_str(), fontColor);
					tempRect.y = i*fontSize, tempRect.w = tempSurf->w, tempRect.h = tempSurf->h;
					SDL_FillRect(surf, &tempRect, SDL_MapRGBA(surf->format,0,0,0,255));
					SDL_BlitSurface(tempSurf, NULL, surf, &tempRect);
					SDL_FreeSurface(tempSurf);
				}
				//=== cursor display
				int w, h;
				TTF_SizeText(font, "_", &w, &h);
				if (timeFlag) {
					curSurf = TTF_RenderText_Blended(font, "_", curColor);
				} else {
					curSurf = TTF_RenderText_Blended(font, " ", curColor);
				}
				curRect.x = w*curCol;//(curSurf->h)*curCol;
				curRect.y = h*curRow;//(curSurf->w)*curRow;
				SDL_BlitSurface(curSurf, NULL, surf, &curRect);
				SDL_FreeSurface(curSurf);
		}
		void onClick() {

		}
		~EditBox() {}
};

class SideBox : public SubBox
{
	public:
		//=== font
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 15;

		//=== save button
		SDL_Surface *saveBtnSurf;
		SDL_Rect saveBtnRect = {0,0,0,0};
		//=== new button
		SDL_Surface *newBtnSurf;
		SDL_Rect newBtnRect = {0,0,0,0};

		void showSurf(int winW, int winH, TTF_Font *font) {
			surf = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
			SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format,50,0,0,255));
				//=== save button
				saveBtnSurf = TTF_RenderUTF8_Blended(font, "Save", fontColor);
				SDL_BlitSurface(saveBtnSurf, NULL, surf, &saveBtnRect);
				//=== new button
				newBtnSurf = TTF_RenderUTF8_Blended(font, "New", fontColor);
				newBtnRect.y = fontSize;
				SDL_BlitSurface(newBtnSurf, NULL, surf, &newBtnRect);
		}
		~SideBox() {}
};

class DownBox : public SubBox
{
	public:
		void showSurf(int winW, int winH, TTF_Font *font) {
			surf = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
			SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format,0,0,50,255));
				//=== do the content work...
		}
		~DownBox() {}
};