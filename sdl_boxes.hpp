#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

enum MouseState
{
	MOUSE_EDITBOX_CLICK,
	MOUSE_SIDEBOX_CLICK,
	MOUSE_DOWNBOX_CLICK,
	MOUSE_EDITBOX_HOVER,
	MOUSE_SIDEBOX_HOVER,
	MOUSE_DOWNBOX_HOVER
};

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
class Mouse
{
	public:
		int x;
		int y;
		bool press = false;
		bool click = false;
		int onBox = 0; //= 0, 1, 2, 3
};

class Button
{
	public:
		SDL_Texture *texture;
		SDL_Rect rect = {0,0,0,0};
		SDL_Surface *surf;

		SDL_Rect margin = {0,0,0,0};
		SDL_Rect border = {0,0,0,0};
		SDL_Rect padding = {0,0,0,0};
		SDL_Rect content = {0,0,0,0};
		int marginSize = 5;
		int borderSize = 3;
		int paddingSize = 5;

		string text;
		int fontSize = 10;
		SDL_Color fontColor = {255,255,255,0xFF};
		SDL_Color backColor = {0,0,0,0};

		Button() : text("") {}
		Button(string t) : text(t) {}
		void setBtn_location(int x, int y) {
			rect.x = x, rect.y = y;
		}
		void setBtn_draw(SDL_Renderer *renderer, TTF_Font *font) {
			//=== setup margin, border, padding Rect.
			SDL_Surface *tsurf = TTF_RenderUTF8_Blended(font, text.c_str(), fontColor);
			SDL_Surface *margSurf;
			SDL_Surface *bordSurf;
			SDL_Surface *paddSurf;
			margin.w = tsurf->w + 2*marginSize + 2*borderSize + 2*paddingSize;
			margin.h = tsurf->h + 2*marginSize + 2*borderSize + 2*paddingSize;
			border.x = marginSize, border.y = marginSize;
			border.w = tsurf->w + 2*marginSize + 2*borderSize;
			border.h = tsurf->h + 2*marginSize + 2*borderSize;
			padding.x = marginSize+borderSize, padding.y = marginSize+borderSize;
			padding.w = tsurf->w + 2*marginSize;
			padding.h = tsurf->h + 2*marginSize;
			content.x = marginSize+borderSize+paddingSize;
			content.y = marginSize+borderSize+paddingSize;
			content.w = tsurf->w, content.h = tsurf->h;
			//=== set button surf
			surf = SDL_CreateRGBSurface(0, margin.w,margin.h, 32, 0,0,0,0);
			//=== set button rect
			rect.w = surf->w, rect.h = surf->h;
			//=== blit button surfs
			margSurf = SDL_CreateRGBSurface(0, margin.w,margin.h, 32, 0,0,0,0);
			bordSurf = SDL_CreateRGBSurface(0, border.w,border.h, 32, 0,0,0,0);
			paddSurf = SDL_CreateRGBSurface(0, padding.w,padding.h, 32, 0,0,0,0);
			SDL_FillRect(bordSurf, NULL, SDL_MapRGBA(bordSurf->format,255,255,255,255));
			SDL_FillRect(paddSurf, NULL, SDL_MapRGBA(paddSurf->format,backColor.r,backColor.g,backColor.b,backColor.a));
			SDL_BlitSurface(margSurf, NULL, surf, &margin);
			SDL_BlitSurface(bordSurf, NULL, surf, &border);
			SDL_BlitSurface(paddSurf, NULL, surf, &padding);
			SDL_BlitSurface(tsurf, NULL, surf, &content);

			//=== make it into texture
			texture = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			//=== free surfs, texture
			SDL_FreeSurface(margSurf);
			SDL_FreeSurface(bordSurf);
			SDL_FreeSurface(paddSurf);
			SDL_FreeSurface(tsurf);
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(texture);
		}
		void onInteract(Mouse m) {
			if(m.x>rect.x+marginSize && m.x<rect.x+border.w && m.y>rect.y+marginSize && m.y<rect.y+rect.h) {
				if(m.press == false) {
					backColor = {100,100,100,255};
				} else { //= if m.press == true
					backColor = {150,150,150,255};
				}
			} else {
				if(m.click) {
					cout<<"click!"<<endl;
				}
				backColor = {0,0,0,255};
			}
		}
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

class PopUp
{
	public:
		SDL_Texture *texture;
		SDL_Surface *surf;
		SDL_Rect rect = {0,0,0,0};
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
		TTF_Font *font;
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 15;

		//=== buttons
		Button saveBtn{"Save"};
		Button newBtn{"New"};

		void setSurf_Menu(TTF_Font *font, SDL_Renderer *renderer, SDL_Rect rect, Mouse mouse) {
			// SDL_Surface *tempInnerSurf;
			// SDL_Surface *tempSurf;
			// SDL_Rect tempRect = {0,0,0,0};
			rect.x += border;
			rect.y += border;
			//=== button setup
			saveBtn.setBtn_location(rect.x, rect.y);
			newBtn.setBtn_location(rect.x+saveBtn.margin.w, rect.y);
			//===button mouse hover/click setup
			saveBtn.onInteract(mouse);
			newBtn.onInteract(mouse);
			//===button display
			saveBtn.setBtn_draw(renderer, font);
			newBtn.setBtn_draw(renderer, font);

			//=== img display
			// SDL_Texture *sometext = IMG_LoadTexture(renderer, "ball.png");
			// int w, h;
			// SDL_QueryTexture(sometext, NULL, NULL, &w,&h);
			// SDL_Rect somerect = {rect.x,rect.y,w,h};
			// SDL_RenderCopy(renderer, sometext, NULL, &somerect);
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