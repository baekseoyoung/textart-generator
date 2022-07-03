#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <windows.h>

#include "sdl_boxes.hpp"
#include "sdl_events.hpp"
#include "win_filemanage.hpp"

#include "asciicalculate.hpp"

using namespace std;

int main(int argc, char **argv) 
{
	//=== sdl window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	int winW = 700;
	int winH = 500;
	// SDL_Rect mainRect = {0,0,winW,winH};

	window = SDL_CreateWindow(
		"ASCII Art Editor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        winW, winH,
        SDL_WINDOW_RESIZABLE
	);
	if(window == NULL) {
		cout << "Failed to create window\n";
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);

	//=== sdl ttf
	TTF_Init();
	TTF_Font *font;
	string fontPath = "assets/Ubuntu_Mono/UbuntuMono-Regular.ttf";
	int fontSize = 20;
	font = TTF_OpenFont(fontPath.c_str(), fontSize);

	//=== sdl image
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

	//=== set textures, rect, surf
	SDL_Texture *editTexture;
	SDL_Texture *sideTexture;
	SDL_Texture *downTexture;
	// SDL_Texture *scrollbar1Texture;
	// SDL_Texture *scrollbar2Texture;

	int sideSize = 200;
	int downSize = 200;
	int scrollbarSize = 20;
	SDL_Rect editRect = {0, 0, winW-sideSize-scrollbarSize, winH-downSize-scrollbarSize};
	SDL_Rect sideRect = {winW-sideSize, 0, sideSize, winH};
	SDL_Rect downRect = {0, winH-downSize, winW-sideSize, downSize};
	// SDL_Rect scrollbar1Rect = {editRect.w, 0, scrollbarSize, editRect.h};
	// SDL_Rect scrollbar2Rect = {0, editRect.h, editRect.w, scrollbarSize};

	EditBox editBox;
	SideBox sideBox;
	DownBox downBox;
	editBox.changeRectSizes(editRect);
	sideBox.changeRectSizes(sideRect);
	downBox.changeRectSizes(downRect);

	//=== textart obj & cursor timing
	TextArt textArt(10, 10);
	SDL_StartTextInput();

	time_t timeprv, timenow;
	float timecha;
	timeprv = time(NULL);
	bool timeFlag = true;

	//=== mouse setup
	// MouseState mouse;
	Mouse mouse;



	//=== main loop
	bool quit = false;
	SDL_Event e;
	while(!quit)
	{	
		//=== reset variables
		mouse.click = false;
		//=== event handles
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT) {
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN) {
				//=== other keyboard events
				timeprv = time(NULL); //=== visible cursor during keydown
				if(SDL_GetModState() & KMOD_CTRL) { //=== ctrl
					switch(e.key.keysym.sym) {
						case SDLK_s:
							// func_write_file(&text);
							break;
						case SDLK_z:
							cout<<"ctrlz"<<endl;
							break;
						case SDLK_o:
							// func_win_open_dialog(&text);
							// textRow = sizeof(text);
							// textCol = sizeof(text[0]);
							// editbox.curRow = 0;
							// editbox.curCol = 0;
							break;
					}
				} else { //=== text input related
					switch(e.key.keysym.sym) {
						case SDLK_BACKSPACE:
							textArt.text[textArt.rowCur][textArt.colCur] = ' ';
							break;
						case SDLK_RETURN:
							cout<<"line break"<<endl;
							break;
						case SDLK_UP:
							if (textArt.rowCur > 0) {
								textArt.rowCur -= 1;
							}
							break;
						case SDLK_DOWN:
							if (textArt.rowCur+1 < textArt.rowSize) {
								textArt.rowCur += 1;
							}
							break;
						case SDLK_LEFT:
							if (textArt.colCur > 0) {
								textArt.colCur -= 1;
							}
							break;
						case SDLK_RIGHT:
							if (textArt.colCur+1 < textArt.colSize) {
								textArt.colCur += 1;
							}
							break;
					}
				}
			}
			else if(e.type == SDL_TEXTINPUT) { //=== editbox text input
				textArt.text[textArt.rowCur][textArt.colCur] = (e.text.text)[0];
				// text[editbox.curRow][editbox.curCol] = (e.text.text)[0];
				// if (editbox.curCol+1 >= textCol) {
				// 	if (editbox.curRow+1 >= textRow) {
				// 		//
				// 	} else {
				// 		editbox.curRow += 1;
				// 		editbox.curCol = 0;
				// 	}
				// } else {
				// 	editbox.curCol += 1;
				// }
			}
			else if(e.type == SDL_WINDOWEVENT) {
				//=== on window resize
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					SDL_GetWindowSize(window, &winW, &winH);

					editRect.w = winW-sideSize-scrollbarSize, editRect.h = winH-downSize-scrollbarSize;
					sideRect.x = winW-sideSize, sideRect.w = sideSize, sideRect.h = winH;
					downRect.y = winH-downSize, downRect.w = winW-sideSize, downRect.h = downSize;
					// scrollbar1Rect.x = editRect.w, scrollbar1Rect.h = editRect.h;
					// scrollbar2Rect.y = editRect.h, scrollbar2Rect.w = editRect.w;

					editBox.changeRectSizes(editRect);
					sideBox.changeRectSizes(sideRect);
					downBox.changeRectSizes(downRect);
				}
			}
			else if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				//=== mouse event on each surface
				SDL_GetMouseState(&mouse.x, &mouse.y);
				if(e.type == SDL_MOUSEBUTTONDOWN) {
					mouse.press = true;
				}
				if(e.type == SDL_MOUSEBUTTONUP) {
					if(mouse.press == true) {
						mouse.click = true;
					}
					mouse.press = false;
				}
				// if(mx>editRect.x && mx<editRect.x+editRect.w && my>editRect.y && my<editRect.y+editRect.h) { // on green
				// 	// cout<<"green"<<endl;
				// 	mouse = MOUSE_EDITBOX_CLICK;
				// }
				// if (mx>sideRect.x && mx<sideRect.x+sideRect.w && my>sideRect.y && my<sideRect.y+sideRect.h) { // on red
				// 	// cout<<"red"<<endl;
				// 	mouse = MOUSE_SIDEBOX_CLICK;
				// }
				// if (mx>downRect.x && mx<downRect.x+downRect.w && my>downRect.y && my<downRect.y+downRect.h) { // on blue
				// 	// cout<<"blue"<<endl;
				// 	mouse = MOUSE_DOWNBOX_CLICK;
				// }
				// cout<<mouse<<endl;
				// cout<<mx<<" "<<my<<endl;
			}
		}
		//=== clear & render texture
		SDL_RenderClear(renderer);

		//=== set surf of each elements
		// screen = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
		// SDL_FillRect(screen, &mainRect, SDL_MapRGBA(screen->format,100,100,100,255));
		// texture = SDL_CreateTextureFromSurface(renderer, screen);
		// SDL_RenderCopy(renderer, texture, NULL, &mainRect);
		editBox.surf = SDL_CreateRGBSurface(0, editRect.w,editRect.h, 32, 0,0,0,0);
		sideBox.surf = SDL_CreateRGBSurface(0, sideRect.w,sideRect.h, 32, 0,0,0,0);
		downBox.surf = SDL_CreateRGBSurface(0, downRect.w,downRect.h, 32, 0,0,0,0);
		editBox.setSurf_BoxBorder(editRect, SDL_MapRGBA(editBox.surf->format,0,150,0,255));
		sideBox.setSurf_BoxBorder(sideRect, SDL_MapRGBA(sideBox.surf->format,150,0,0,255));
		downBox.setSurf_BoxBorder(downRect, SDL_MapRGBA(downBox.surf->format,0,0,150,255));

		editBox.setSurf_Editor(textArt, font, timeFlag);

		//=== into texture for each element
		editTexture = SDL_CreateTextureFromSurface(renderer, editBox.surf);
		sideTexture = SDL_CreateTextureFromSurface(renderer, sideBox.surf);
		downTexture = SDL_CreateTextureFromSurface(renderer, downBox.surf);

		SDL_RenderCopy(renderer, editTexture, NULL, &editRect);
		SDL_RenderCopy(renderer, sideTexture, NULL, &sideRect);
		SDL_RenderCopy(renderer, downTexture, NULL, &downRect);

		sideBox.setSurf_Menu(font, renderer, sideRect, mouse);

		SDL_FreeSurface(editBox.surf);
		SDL_FreeSurface(sideBox.surf);
		SDL_FreeSurface(downBox.surf);
		SDL_DestroyTexture(editTexture);
		SDL_DestroyTexture(sideTexture);
		SDL_DestroyTexture(downTexture);

		//=== time count for cursor
		timenow = time(NULL);
		timecha = (float)(timenow-timeprv);
		if(timecha>=0 && timecha<1) {
			timeFlag = true;
		} else if(timecha>=1 && timecha<2) {
			timeFlag = false;
		} else {
			timeprv = time(NULL);
		}

		//=== render the texture
		SDL_RenderPresent(renderer);
	}

	//=== quit
	IMG_Quit();
	SDL_StopTextInput();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}