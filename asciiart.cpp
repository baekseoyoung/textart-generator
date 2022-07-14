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
// #include "sdl_events.hpp"
#include "win_filemanage.hpp"

#include "asciicalculate.hpp"

using namespace std;

int main(int argc, char **argv) 
{
	//=== sdl window & renderer
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	int winW = 700;
	int winH = 500;

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



	//=== set basic window's var, box, elements.
	int sideSize = 200;
	int downSize = 200;

	EditBox editBox;
	SideBox sideBox;
	DownBox downBox;
	editBox.resizeBox(0, 0, winW-sideSize, winH-downSize);
	sideBox.resizeBox(winW-sideSize, 0, sideSize, winH);
	downBox.resizeBox(0, winH-downSize, winW-sideSize, downSize);
	editBox.recolorBox(0,150,0,255);
	sideBox.recolorBox(150,0,0,255);
	downBox.recolorBox(0,0,150,255);

	//temp tab
	sideBox.addTab("File");
	sideBox.addTab("Edit");

	//=== init: text art obj, cursor timing, mouse.
	TextArt textArt(10, 20);
	SDL_StartTextInput();

	time_t timeprv, timenow;
	float timecha;
	timeprv = time(NULL);
	bool timeFlag = true;

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
					editBox.resizeBox(0, 0, winW-sideSize, winH-downSize);
					sideBox.resizeBox(winW-sideSize, 0, sideSize, winH);
					downBox.resizeBox(0, winH-downSize, winW-sideSize, downSize);
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

		//=== draw basic boxes in windows.
		editBox.drawBox(renderer, font);
		sideBox.drawBox(renderer, font);
		downBox.drawBox(renderer, font);

		//=== draw boxes's contents. (drawing in textures.)
		editBox.onMouseEvent(mouse, &textArt, font);
		editBox.drawContent(renderer, textArt, font, timeFlag, mouse);
		
		sideBox.drawContent(renderer, font, mouse);

		//temp bar....
		ScrollBar bar("vert");
		bar.setupScrollBarLocation(winW-bar.scrollSize, 0, winH, font);
		bar.drawScrollBar(renderer, font, mouse);

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