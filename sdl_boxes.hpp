#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "sdl_events.hpp"

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
		bool hover = true;
		bool press = false;
		bool click = false;
		SDL_Rect mScreen = {0,0,0,0}; //=== click-able screen area.

		void onEvent(SDL_Event *e) {
			SDL_GetMouseState(&x, &y);
			if(mScreen.x<x && mScreen.y<y && mScreen.x+mScreen.w>x && mScreen.y+mScreen.h>y) {
				hover = true;
				if(e->type == SDL_MOUSEBUTTONDOWN) {
					press = true;
				}
				if(e->type == SDL_MOUSEBUTTONUP) {
					if(press == true) {
						click = true;
					}
					press = false;
				}
			} else {
				hover = false;
			}
		}
		void setupMScreen(SDL_Rect rect) {
			mScreen.x = rect.x;
			mScreen.y = rect.y;
			mScreen.w = rect.w;
			mScreen.h = rect.h;
		}
};

class Button
{
	public:
		SDL_Texture *texture;
		SDL_Surface *surf;
		SDL_Rect rect = {0,0,0,0}; //=== rect to render-copy texture.

		SDL_Rect margin = {0,0,0,0};
		SDL_Rect border = {0,0,0,0};
		SDL_Rect padding = {0,0,0,0};
		SDL_Rect content = {0,0,0,0};
		int margSize = 5;
		int bordSize = 3;
		int paddSize = 5;

		string text;
		SDL_Color fontColor = {255,255,255,0xFF};
		SDL_Color bordColor = {255,255,255,255};
		SDL_Color backColor = {0,0,0,0};

		Button() : text("") {}
		Button(string t) : text(t) {}

		//=== funcs for drawing button.
		void drawButton(SDL_Renderer *renderer, TTF_Font *font) {
			//=== color button margin, border, padding.
			surf = SDL_CreateRGBSurface(0, margin.w,margin.h, 32, 0,0,0,0);
			SDL_FillRect(surf, &margin, SDL_MapRGBA(surf->format,0,0,0,0));
			SDL_FillRect(surf, &border, SDL_MapRGBA(surf->format,bordColor.r,bordColor.g,bordColor.b,bordColor.a));
			SDL_FillRect(surf, &padding, SDL_MapRGBA(surf->format,backColor.r,backColor.g,backColor.b,backColor.a));
			//=== blit text.
			SDL_Surface *tempSurf = TTF_RenderUTF8_Blended(font, text.c_str(), fontColor);
			SDL_BlitScaled(tempSurf, NULL, surf, &content);
			SDL_FreeSurface(tempSurf);

			//=== into texture
			texture = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			//=== free
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(texture);
		}
		void onMouseEvent(Mouse m, void (*func)(bool)) {
			if(margSize == 0) {
				//=== if no margin
				if(m.x>rect.x && m.x<rect.x+rect.w && m.y>rect.y && m.y<rect.y+rect.h) {
					if(m.press == false) {
						backColor = {100,100,100,255};
						if(m.click) {
							if(func != NULL) {
								func(m.click);
							}
						}
					} else { //= if m.press == true
						backColor = {150,150,150,255};
					}
				} else {
					backColor = {0,0,0,255};
				}
			} else {
				//=== if there is margin
				if(m.x>rect.x+margSize && m.x<rect.x+margSize+border.w && m.y>rect.y+margSize && m.y<rect.y+margSize+border.h) {
					if(m.press == false) {
						backColor = {100,100,100,255};
						if(m.click) {
							if(func != NULL) {
								func(m.click);
							}
						}
					} else { //= if m.press == true
						backColor = {150,150,150,255};
					}
				} else {
					backColor = {0,0,0,255};
				}
			}
		}

		//=== funcs for changing/setup button properties.
		void setupButtonLocation(int x, int y, TTF_Font *font) {
			int w, h;
			TTF_SizeText(font, text.c_str(), &w, &h);
			//=== margin
			margin.w = w + 2*margSize + 2*bordSize + 2*paddSize;
			margin.h = h + 2*margSize + 2*bordSize + 2*paddSize;
			//=== border
			border.x = margSize;
			border.y = margSize;
			border.w = w + 2*bordSize + 2*paddSize;
			border.h = h + 2*bordSize + 2*paddSize;
			//=== padding
			padding.x = margSize + bordSize;
			padding.y = margSize + bordSize;
			padding.w = w + 2*paddSize;
			padding.h = h + 2*paddSize;
			//=== content
			content.x = margSize + bordSize + paddSize;
			content.y = margSize + bordSize + paddSize;
			content.w = w;
			content.h = h;
			//=== rect
			rect.x = x, rect.y = y;
			rect.w = margin.w, rect.h = margin.h;
		}
		void setupButtonLocation(int x, int y, int wfix, int hfix, TTF_Font *font) {
			// margSize = 0;
			//=== rect
			rect.x = x, rect.y = y;
			rect.w = wfix, rect.h = hfix;
			//=== margin
			margin.w = wfix;
			margin.h = hfix;
			//=== border
			border.x = margSize;
			border.y = margSize;
			border.w = wfix - 2*margSize;
			border.h = hfix - 2*margSize;
			//=== padding
			padding.x = margSize + bordSize;
			padding.y = margSize + bordSize;
			padding.w = wfix - 2*margSize - 2*bordSize;
			padding.h = hfix - 2*margSize - 2*bordSize;
			//=== content
			content.x = margSize + bordSize + paddSize;
			content.y = margSize + bordSize + paddSize;
			content.w = wfix - 2*margSize - 2*bordSize - 2*paddSize;
			content.h = hfix - 2*margSize - 2*bordSize - 2*paddSize;
		}
		void setupButtonText(string s) {
			text.clear();
			text = s;
		}
		void setupButtonMBP(int m, int b, int p) {
			margSize = m;
			bordSize = b;
			paddSize = p;
		}
		void setupButtonColor(SDL_Color c) { //int r, int g, int b, int a
			bordColor.r = c.r;
			bordColor.g = c.g;
			bordColor.b = c.b;
			bordColor.a = c.a;
		}
		~Button() {}
};

class Tabs
{
	public:
		vector<string> tabs;
		int current = 0;
		int w = 50;
		int h = 20;
		int borderSize = 5;
		bool deleteable = true;

		//=== draw tabs
		void drawTabs(SDL_Renderer *renderer, TTF_Font *font, Mouse m, SDL_Rect rect, SDL_Color borderColor) {
			for(int i=0; i<(int)tabs.size(); i++) {
				if(i == current) {
					Button tab(tabs[i]);
					tab.setupButtonMBP(0, borderSize, 1);
					tab.setupButtonColor(borderColor);
					tab.setupButtonLocation(rect.x+i*w, rect.y, w, h+borderSize, font);
					tab.padding.h += borderSize;
					tab.onMouseEvent(m, NULL);
					tab.drawButton(renderer, font);
				} else {
					Button tab(tabs[i]);
					tab.setupButtonMBP(0, borderSize, 1);
					tab.setupButtonColor(borderColor);
					tab.setupButtonLocation(rect.x+i*w, rect.y, w, h+borderSize, font);
					tab.onMouseEvent(m, NULL);
					tab.drawButton(renderer, font);
				}
			}
		}
		//=== tab style, type related.
		void setupTabOptions(int num, bool flag) {
			borderSize = num;
			deleteable = flag;
		}
		//=== tab manage functions
		void addTab(string s) {
			// current = tabs.size();
			tabs.push_back(s);
		}
		void changeTab(int num) {
			current = num;
		}
		void delTab(int tabNum) {}
};

class ScrollBar
{
	public:
		SDL_Texture *texture;
		SDL_Surface *surf;
		SDL_Rect rect = {0,0,0,0}; //=== rect to put it.

		//=== scrollbar prop.
		int scrollSize = 30;
		bool scrollType; //=== vert:T, hori:F

		//=== scrollbar buttons.
		Button btn0{""};
		Button btn1;
		Button btn2;

		ScrollBar(string s) {
			if(s == "vert") {
				scrollType = true;
				btn1.setupButtonText("^");
				btn2.setupButtonText("v");
			} else if (s == "hori") {
				scrollType = false;
				btn1.setupButtonText("<");
				btn2.setupButtonText(">");
			}
		}

		//=== draw scrollbar func.
		void drawScrollBar(SDL_Renderer *renderer, TTF_Font *font, Mouse m) {
			//=== mouse event.
			btn1.onMouseEvent(m, NULL);
			btn2.onMouseEvent(m, NULL);
			btn0.onMouseEvent(m, NULL);
			//=== button display.
			btn1.drawButton(renderer, font);
			btn2.drawButton(renderer, font);
			btn0.drawButton(renderer, font);
		}
		void onMouseEvent(Mouse m) {}

		//=== funcs for scrollbar properties.
		void setupScrollBarLocation(int x, int y, int len, TTF_Font *font) {
			rect.x = x, rect.y = y;
			if(scrollType) { //=== vertical.
				rect.w = scrollSize, rect.h = len;
				btn1.setupButtonLocation(rect.x, rect.y, rect.w, rect.w, font);
				btn2.setupButtonLocation(rect.x, rect.y+rect.h-rect.w, rect.w, rect.w, font);
				btn0.setupButtonLocation(rect.x, rect.y+rect.w, rect.w, rect.w, font);
			} else { //=== horizontal.
				rect.w = len, rect.h = scrollSize;
				btn1.setupButtonLocation(rect.x, rect.y, rect.h, rect.h, font);
				btn2.setupButtonLocation(rect.x+rect.w-rect.h, rect.y, rect.h, rect.h, font);
				btn0.setupButtonLocation(rect.x+rect.h, rect.y, rect.h, rect.h, font);
			}
		}
		void setupScrollBarSize(int size) {}
		void setupScrollBarProp() {}
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
		SDL_Texture *texture;
		SDL_Surface *surf;
		SDL_Rect rect = {0,0,0,0};
		SDL_Rect border = {0,0,0,0};
		SDL_Rect outer = {0,0,0,0};
		SDL_Rect inner = {0,0,0,0};

		int borderSize = 3;
		SDL_Color borderColor = {255,255,255,255};

		Tabs boxtabs;

		//=== draw box outline using obj's data saved above.
		void drawBox(SDL_Renderer *renderer, TTF_Font *font, Mouse m) {
			//=== set box surf
			surf = SDL_CreateRGBSurface(0, rect.w,rect.h, 32, 0,0,0,0);
			SDL_FillRect(surf, &border, SDL_MapRGBA(surf->format, borderColor.r,borderColor.g,borderColor.b,borderColor.a));
			SDL_FillRect(surf, &outer, SDL_MapRGBA(surf->format, 0,0,0,255));
			SDL_FillRect(surf, &inner, SDL_MapRGBA(surf->format, 50,50,50,255));
			//=== draw box texture
			texture = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			//=== free
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(texture);
			//=== draw tabs
			boxtabs.drawTabs(renderer, font, m, rect, borderColor);
		}
		//=== change box size-related variables
		void resizeBox(int x, int y, int w, int h) {
			rect.x = x, rect.y = y, rect.w = w, rect.h = h;
			border.x = 0, border.y = boxtabs.h, border.w = w, border.h = h-boxtabs.h;
			//== outer
			outer.x = border.x + borderSize;
			outer.y = border.y + borderSize;
			outer.w = border.w - 2*borderSize;
			outer.h = border.h - 2*borderSize;
			//=== inner
			inner.x = border.x + 2*borderSize;
			inner.y = border.y + 2*borderSize;
			inner.w = border.w - 4*borderSize;
			inner.h = border.h - 4*borderSize;
		}
		//=== change box's color(border)
		void recolorBox(int r, int g, int b, int a) {
			borderColor.r = r;
			borderColor.g = g;
			borderColor.b = b;
			borderColor.a = a;
		}
		~Box() {}
};

class EditBox : public Box
{
	public:
		SDL_Texture *editScreen;

		//=== font var
		int fontSize = 20;
		SDL_Color fontColor = {255,255,255,0xFF};

		//=== cursor var
		SDL_Surface *curSurf;
		SDL_Rect curRect = {0, 0, 0, 0};
		SDL_Color curColor = {0,255,0,0xFF};

		EditBox() {}
		void preset(TTF_Font *font) {
			//=== tabs init.
			boxtabs.setupTabOptions(borderSize, true);
			boxtabs.addTab("File");
			boxtabs.addTab("File");
			boxtabs.addTab("File");
		}
		void drawContent(SDL_Renderer *renderer, TextArt textArt, TTF_Font *font, bool timeFlag, Mouse m) {
			//=== text display
			SDL_Surface *tempSurf;
			// SDL_Rect tempRect = {0,0,0,0};
			SDL_Surface *textSurf;
			SDL_Rect textRect = {0,0,0,0};

			//=== content surf.
			tempSurf = SDL_CreateRGBSurface(0, inner.w,inner.h, 32, 0,0,0,0);
			SDL_FillRect(tempSurf, NULL, SDL_MapRGBA(tempSurf->format,50,50,50,255));

			//=== blit each line. according to mouse hover.
			for(int i=0; i<textArt.rowSize; i++) {
				//=== make the text first
				textSurf = TTF_RenderUTF8_Blended(font, textArt.text[i].c_str(), fontColor);
				textRect.y = i*fontSize, textRect.w = textSurf->w, textRect.h = textSurf->h;
				//=== color text's background.
				if(m.x-inner.x>textRect.x && m.y-inner.y>textRect.y && m.x-inner.x<textRect.x+textRect.w && m.y-inner.y<textRect.y+textRect.h) {
					SDL_Rect charRect = {textRect.x+(textRect.w/textArt.colSize)*(int)((m.x-inner.x)/(textRect.w/textArt.colSize)), textRect.y, textRect.w/textArt.colSize, textRect.h};
					SDL_FillRect(tempSurf, &textRect, SDL_MapRGBA(tempSurf->format,0,0,0,255));
					SDL_FillRect(tempSurf, &charRect, SDL_MapRGBA(tempSurf->format,100,100,100,255));
				}
				else {
					SDL_FillRect(tempSurf, &textRect, SDL_MapRGBA(tempSurf->format,0,0,0,255));
				}
				//=== blit,free text.
				SDL_BlitSurface(textSurf, NULL, tempSurf, &textRect);
				SDL_FreeSurface(textSurf);
			}

			//=== cursor display
			int w, h;
			TTF_SizeText(font, "_", &w, &h);
			if (timeFlag) {
				curSurf = TTF_RenderText_Blended(font, "_", curColor);
			} else {
				curSurf = TTF_RenderText_Blended(font, " ", curColor);
			}
			curRect.x = w*textArt.colCur;
			curRect.y = h*textArt.rowCur;
			SDL_BlitSurface(curSurf, NULL, tempSurf, &curRect);
			SDL_FreeSurface(curSurf);

			//=== edit screen's surf -> texture
			editScreen = SDL_CreateTextureFromSurface(renderer, tempSurf);
			SDL_RenderCopy(renderer, editScreen, NULL, &inner);
			//=== free
			SDL_FreeSurface(tempSurf);
			SDL_DestroyTexture(editScreen);
		}
		void onMouseEvent(Mouse m, TextArt *textArt, TTF_Font *font) {
			//=== click
			if(m.click) {
				int w, h;
				TTF_SizeText(font, "_", &w, &h);
				if(m.x > inner.x && m.y > inner.y && m.x < inner.x+w*textArt->colSize && m.y < inner.y+h*textArt->rowSize) {
					textArt->colCur = (m.x-inner.x) / w;
					textArt->rowCur = (m.y-inner.y) / h;
				}
			}
		}
		~EditBox() {}
};

class SideBox : public Box
{
	public:
		//=== font
		TTF_Font *font;
		SDL_Color fontColor = {255,255,255,0xFF};
		int fontSize = 20;

		//=== buttons
		Button saveBtn{"Save"};
		Button newBtn{"New"};

		SideBox() {}
		void preset(TTF_Font *font) {
			//=== tabs init.
			boxtabs.setupTabOptions(borderSize, false);
			boxtabs.addTab("File");
			boxtabs.addTab("Edit");
			//=== button setup
			saveBtn.setupButtonLocation(rect.x+inner.x, rect.y+inner.y, font);
			newBtn.setupButtonLocation(rect.x+inner.x+saveBtn.margin.w, rect.y+inner.y, font);
		}
		void drawContent(SDL_Renderer *renderer, TTF_Font *font, Mouse m) {
			//===button mouse hover/click setup
			saveBtn.onMouseEvent(m, &func_btn_save);
			newBtn.onMouseEvent(m, &func_btn_new);
			//===button display
			saveBtn.drawButton(renderer, font);
			newBtn.drawButton(renderer, font);
		}
		~SideBox() {}

		// void setSurf_Menu(TTF_Font *font, SDL_Renderer *renderer, SDL_Rect rect, Mouse mouse) {
		// 	//=== img display
		// 	// SDL_Texture *sometext = IMG_LoadTexture(renderer, "ball.png");
		// 	// int w, h;
		// 	// SDL_QueryTexture(sometext, NULL, NULL, &w,&h);
		// 	// SDL_Rect somerect = {rect.x,rect.y,w,h};
		// 	// SDL_RenderCopy(renderer, sometext, NULL, &somerect);
		// }
};

class DownBox : public Box
{
	public:
		DownBox() {}
		void preset(TTF_Font *font) {
			//=== tabs init.
			boxtabs.setupTabOptions(borderSize, false);
			boxtabs.addTab("Animation");
		}
		void drawContent(SDL_Renderer *renderer, TTF_Font *font, Mouse m) {}
		~DownBox() {}
};