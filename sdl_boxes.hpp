#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

//=== text class ===
class TextArt
{
	public:
		vector<string> text;
		int rowSize = 0;
		int colSize = 0;
		int rowCur = 0;
		int colCur = 0;
		TextArt(int r, int c) {
			rowSize = r;
			colSize = c;
			string temp;
			for(int i=0; i<rowSize; i++) {
				temp = "";
				for(int j=0; j<colSize; j++) {
					temp += " ";
				}
				text.push_back(temp);
			}
		}
};

//=== element classes ===

class Button
{
	public:
		SDL_Surface *surf;
		SDL_Rect margin = {0,0,0,0};
		SDL_Rect border = {0,0,0,0};
		SDL_Rect padding = {0,0,0,0};
		SDL_Rect content = {0,0,0,0};

		string text;
		SDL_Color borderColor = {255,255,255,0xFF};

		int marginSize = 5;
		int borderSize = 5;
		int paddingSize = 5;

		Button(string t, Uint32 pixel, int marginS, int borderS, int paddingS) {
			text = t;
			// if(pixel != NULL) {
			// 	borderColor = pixel;
			// }
			// if(marginS != NULL) {
			// 	marginSize = marginS;
			// }
			// if(borderS != NULL) {
			// 	borderSize = borderS;
			// }
			// if(paddingS != NULL) {
			// 	paddingSize = paddingS;
			// }
		}
		void showButton() {}
		void onClick() {}
		~Button() {}
};

class ScrollBar
{
	public:
		SDL_Surface *surf;
		SDL_Rect rect = {0,0,0,0};
		ScrollBar(int w, int h) {
			// if(w > h) {
			// 	Button bttn1("<");
			// 	Button bttn2(">");
			// } else {
			// 	Button bttn1("^");
			// 	Button bttn2("v");
			// }
		}
};

//=== box classes ===
class Box
{
	public:
		SDL_Surface *surf;
		SDL_Rect outer = {0,0,0,0};
		SDL_Rect inner = {0,0,0,0};
		int border = 5;
		void setSurf_BoxBorder(SDL_Rect screenRect, Uint32 pixel) {
			SDL_FillRect(surf, NULL, pixel);
			SDL_FillRect(surf, &outer, SDL_MapRGBA(surf->format,0,0,0,255));
			// SDL_FillRect(surf, &inner, SDL_MapRGBA(surf->format,50,50,50,255));
		}
		void changeRectSizes(SDL_Rect screenRect) {
			outer.x = border;
			outer.y = border;
			outer.w = screenRect.w - 2*border;
			outer.h = screenRect.h - 2*border;
			inner.x = 2*border;
			inner.y = 2*border;
			inner.w = screenRect.w - 4*border;
			inner.h = screenRect.h - 4*border;
		}
		~Box() {}
};

class EditBox : public Box
{
	public:
		//=== font var
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 20;

		//=== cursor var
		SDL_Surface *curSurf;
		SDL_Rect curRect = {0, 0, 0, 0};
		SDL_Color curColor = {0,255,0,0xFF};

		void setSurf_Editor(TextArt textArt, TTF_Font *font, bool timeFlag) {
			SDL_Surface *tempInnerSurf;
			SDL_Surface *tempSurf;
			SDL_Rect tempRect = {0,0,0,0};
			//=== text display
			tempInnerSurf = SDL_CreateRGBSurface(0, inner.w,inner.h, 32, 0,0,0,0);
			SDL_FillRect(tempInnerSurf, NULL, SDL_MapRGBA(tempInnerSurf->format,50,50,50,255));
			for(int i=0; i<(int)textArt.text.size(); i++) {
				string line = textArt.text[i];
				tempSurf = TTF_RenderUTF8_Blended(font, line.c_str(), fontColor);
				tempRect.y = i*fontSize, tempRect.w = tempSurf->w, tempRect.h = tempSurf->h;
				SDL_FillRect(tempInnerSurf, &tempRect, SDL_MapRGBA(surf->format,0,0,0,255));
				SDL_BlitSurface(tempSurf, NULL, tempInnerSurf, &tempRect);
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
			curRect.x = w*textArt.colCur;//(curSurf->h)*curCol;
			curRect.y = h*textArt.rowCur;//(curSurf->w)*curRow;
			SDL_BlitSurface(curSurf, NULL, tempInnerSurf, &curRect);
			SDL_FreeSurface(curSurf);
			//=== blit to screen surf
			SDL_BlitSurface(tempInnerSurf, NULL, surf, &inner);
			SDL_FreeSurface(tempInnerSurf);
		}
		void onClick() {}
		~EditBox() {}
};

class SideBox : public Box
{
	public:
		//=== font
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 15;

		//=== buttons
		// SDL_Surface *saveBtnSurf;
		SDL_Rect saveRect = {0,0,0,0};
		// SDL_Surface *newBtnSurf;
		SDL_Rect newRect = {0,0,0,0};

		void setSurf_Menu(TTF_Font *font, SDL_Renderer *renderer) {
			// SDL_Surface *tempInnerSurf;
			// SDL_Surface *tempSurf;
			// SDL_Rect tempRect = {0,0,0,0};
			//=== button display
			/*SDL_Surface *image = IMG_Load("ball.png");
			if (!image) {
				cout<<"not loaded"<<endl;
			}
			SDL_Texture *sometext = SDL_CreateTextureFromSurface(renderer, image);
			SDL_FreeSurface(image);
			SDL_RenderCopy(renderer, sometext, NULL, &inner);*/

			SDL_Texture *sometext = IMG_LoadTexture(renderer, "ball.png");
			int w, h;
			SDL_QueryTexture(sometext, NULL, NULL, &w,&h);
			SDL_Rect somerect = {0,0,w,h};
			SDL_RenderCopy(renderer, sometext, NULL, &somerect);
		}
		~SideBox() {}
};

class DownBox : public Box
{
	public:
		void showSurf(int winW, int winH, TTF_Font *font) {
			surf = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
			SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format,0,0,50,255));
				//=== do the content work...
		}
		~DownBox() {}
};