#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>

#include "sdl_boxes.hpp"
#include "sdl_events.hpp"
#include "win_filemanage.hpp"

#include "asciicalculate.hpp"

using namespace std;

int main(int argc, char **argv) 
{
	//=== set variables
	int winW = 700;
	int winH = 500;
	int sideSize = 200;
	int downSize = 200;
	int scrollbar = 10;
	int editc[4] = {0, 0, winW-sideSize-scrollbar, winH-downSize-scrollbar};
	int sidec[4] = {winW-sideSize, 0, sideSize, winH};
	int downc[4] = {0, winH-downSize, winW-sideSize, downSize};

	//=== sdl window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Surface *screen;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect mainrect = {0, 0, winW, winH};

	window = SDL_CreateWindow(
		"untitled",
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
	string fontPath = "assets/unifont-14.0.02.ttf";
	int fontSize = 20;
	font = TTF_OpenFont(fontPath.c_str(), fontSize);

	//=== set boxes
	EditBox editbox;
	SideBox sidebox;
	DownBox downbox;
	editbox.setBoxSize(editc);
	sidebox.setBoxSize(sidec);
	downbox.setBoxSize(downc);

	//=== text & cursor
	vector<string> text;
	int textRow = 10;
	int textCol = 10;
	func_init_text(textRow, textCol, &text);
	SDL_StartTextInput();

	time_t timeprv, timenow;
	float timecha;
	timeprv = time(NULL);
	bool timeFlag = true;



	//=== main loop
	bool quit = false;
	SDL_Event e;
	while(!quit)
	{	
		//=== event handles
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT) {
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN) {
				//=== other keyboard events
				timeprv = time(NULL); //=== visible cursor during keydown
				// func_keyboard_event(e);
				if(SDL_GetModState() & KMOD_CTRL) { //=== ctrl
					switch(e.key.keysym.sym) {
						case SDLK_s:
							func_write_file(&text);
							break;
						case SDLK_z:
							cout<<"ctrlz"<<endl;
							break;
						case SDLK_o:
							func_win_open_dialog(&text);
							textRow = sizeof(text);
							textCol = sizeof(text[0]);
							editbox.curRow = 0;
							editbox.curCol = 0;
							break;
					}
				} else {
					switch(e.key.keysym.sym) { //=== text input related
						case SDLK_BACKSPACE:
							text[editbox.curRow][editbox.curCol] = ' ';
							break;
						case SDLK_RETURN:
							cout<<"line break"<<endl;
							break;
						case SDLK_UP:
							if (editbox.curRow > 0) {
								editbox.curRow -= 1;
							}
							break;
						case SDLK_DOWN:
							if (editbox.curRow+1 < textRow) {
								editbox.curRow += 1;
							}
							break;
						case SDLK_LEFT:
							if (editbox.curCol > 0) {
								editbox.curCol -= 1;
							}
							break;
						case SDLK_RIGHT:
							if (editbox.curCol+1 < textCol) {
								editbox.curCol += 1;
							}
							break;
					}
				}
			}
			else if(e.type == SDL_TEXTINPUT) {
				//=== editbox text input
				text[editbox.curRow][editbox.curCol] = (e.text.text)[0];
				if (editbox.curCol+1 >= textCol) {
					if (editbox.curRow+1 >= textRow) {
						//
					} else {
						editbox.curRow += 1;
						editbox.curCol = 0;
					}
				} else {
					editbox.curCol += 1;
				}
			}
			else if(e.type == SDL_WINDOWEVENT) {
				//=== window resize
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					SDL_GetWindowSize(window, &winW, &winH);
					mainrect.w = winW, mainrect.h = winH;
					editc[0] = 0, editc[1] = 0, editc[2] = winW-sideSize-scrollbar, editc[3] = winH-downSize-scrollbar;
					sidec[0] = winW-sideSize, sidec[1] = 0, sidec[2] = sideSize, sidec[3] = winH;
					downc[0] = 0, downc[1] = winH-downSize, downc[2] = winW-sideSize, downc[3] = downSize;
					editbox.setBoxSize(editc);
					sidebox.setBoxSize(sidec);
					downbox.setBoxSize(downc);
				}
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				//=== mouse event on each surface
				int x, y;
				SDL_GetMouseState(&x, &y);
				if(x>editc[0] && x<editc[0]+editc[2] && y>editc[1] && y<editc[1]+editc[3]) { // on green
					cout<<"green"<<endl;
				}
				if (x>sidec[0] && x<sidec[0]+sidec[2] && y>sidec[1] && y<sidec[1]+sidec[3]) { // on red
					cout<<"red"<<endl;
				}
				if (x>downc[0] && x<downc[0]+downc[2] && y>downc[1] && y<downc[1]+downc[3]) { // on blue
					cout<<"blue"<<endl;
				}
			}
		}
		//=== clear & render screen, render texture
		SDL_RenderClear(renderer);
		screen = SDL_CreateRGBSurface(0, winW,winH, 32, 0,0,0,0);
		SDL_FillRect(screen, &mainrect, SDL_MapRGBA(screen->format,0,0,0,255));
		//=== draw each box on screen
		editbox.showBox(screen, SDL_MapRGBA(screen->format,0,150,0,255));
		sidebox.showBox(screen, SDL_MapRGBA(screen->format,150,0,0,255));
		downbox.showBox(screen, SDL_MapRGBA(screen->format,0,0,150,255));

		//=== show surf of each box, blit to screen, free surf.
		timenow = time(NULL);
		timecha = (float)(timenow-timeprv);
		if(timecha>=0 && timecha<1) {
			timeFlag = true;
		} else if(timecha>=1 && timecha<2) {
			timeFlag = false;
		} else {
			timeprv = time(NULL);
		}
		editbox.showSurf(winW, winH, text, font, timeFlag);
		SDL_BlitSurface(editbox.surf, &editbox.rect, screen, &editbox.inner);
		SDL_FreeSurface(editbox.surf);
		sidebox.showSurf(winW, winH, font);
		SDL_BlitSurface(sidebox.surf, &sidebox.rect, screen, &sidebox.inner);
		SDL_FreeSurface(sidebox.surf);
		downbox.showSurf(winW, winH, font);
		SDL_BlitSurface(downbox.surf, &downbox.rect, screen, &downbox.inner);
		SDL_FreeSurface(downbox.surf);

		//=== render the texture
		texture = SDL_CreateTextureFromSurface(renderer, screen);
		SDL_RenderCopy(renderer, texture, NULL, &mainrect);
		SDL_RenderPresent(renderer);
		//=== free them
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(texture);
	}
	//SDL_UpdateWindowSurface(window);

	//=== quit
	SDL_StopTextInput();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}