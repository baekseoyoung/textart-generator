#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "asciicalculate.hpp"
#include "ascii_sty.h"
#include "sdl_events.hpp"

using namespace std;

int main(int argc, char **argv) 
{
	//=== get styles
	MyStyle sty;

	//=== sdl
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Surface *screen;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect mainrect = {0, 0, sty.winW, sty.winH};

	window = SDL_CreateWindow(
		"untitled",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        sty.winW, sty.winH,
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
	font = TTF_OpenFont(sty.fontPath.c_str(), sty.fontSize);

	//=== vars & make init screens
	SDL_Surface *editSurface;
	SDL_Rect editBox = {sty.editc[0], sty.editc[1], sty.editc[2], sty.editc[3]};
	SDL_Rect outerEditRect = {sty.editc[0]+sty.border, sty.editc[1]+sty.border, sty.editc[2]-2*sty.border, sty.editc[3]-2*sty.border};
	SDL_Rect editRect = {sty.editc[0]+2*sty.border, sty.editc[1]+2*sty.border, sty.editc[2]-4*sty.border, sty.editc[3]-4*sty.border};
	SDL_Surface *sideSurface;
	SDL_Rect sideBox = {sty.sidec[0], sty.sidec[1], sty.sidec[2], sty.sidec[3]};
	SDL_Rect outerSideRect = {sty.sidec[0]+sty.border, sty.sidec[1]+sty.border, sty.sidec[2]-2*sty.border, sty.sidec[3]-2*sty.border};
	SDL_Rect sideRect = {sty.sidec[0]+2*sty.border, sty.sidec[1]+2*sty.border, sty.sidec[2]-4*sty.border, sty.sidec[3]-4*sty.border};
	SDL_Surface *downSurface;
	SDL_Rect downBox = {sty.downc[0], sty.downc[1], sty.downc[2], sty.downc[3]};
	SDL_Rect outerDownRect = {sty.downc[0]+sty.border, sty.downc[1]+sty.border, sty.downc[2]-2*sty.border, sty.downc[3]-2*sty.border};
	SDL_Rect downRect = {sty.downc[0]+2*sty.border, sty.downc[1]+2*sty.border, sty.downc[2]-4*sty.border, sty.downc[3]-4*sty.border};
	SDL_Surface *tempSurface;
	SDL_Rect tempRect;
	string screenText = "";//= func_init_pixel(sty.txtW, sty.txtH);
	SDL_StartTextInput();

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
				screenText = func_sdl_keyboard_event(e, screenText);
			}
			else if(e.type == SDL_TEXTINPUT) {
				screenText += e.text.text;
			}
			else if(e.type == SDL_WINDOWEVENT) {
				/*if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					sty.func_window_update(window);
				}*/
			}
		}
		//=== clear & render screen, render texture
		SDL_RenderClear(renderer);
		screen = SDL_CreateRGBSurface(0, sty.winW,sty.winH, 32, 0,0,0,0);
		SDL_FillRect(screen, &mainrect, SDL_MapRGBA(screen->format,0,0,0,255));
		SDL_FillRect(screen, &editBox, SDL_MapRGBA(screen->format,0,150,0,255));
		SDL_FillRect(screen, &sideBox, SDL_MapRGBA(screen->format,150,0,0,255));
		SDL_FillRect(screen, &downBox, SDL_MapRGBA(screen->format,0,0,150,255));
		SDL_FillRect(screen, &outerEditRect, SDL_MapRGBA(screen->format,0,0,0,255));
		SDL_FillRect(screen, &outerSideRect, SDL_MapRGBA(screen->format,0,0,0,255));
		SDL_FillRect(screen, &outerDownRect, SDL_MapRGBA(screen->format,0,0,0,255));
		//=== surfaces all together
			//=== edit surface
			editSurface = SDL_CreateRGBSurface(0, sty.winW,sty.winH, 32, 0,0,0,0);
			SDL_FillRect(editSurface, &editRect, SDL_MapRGBA(editSurface->format,0,50,0,255));
				tempSurface = TTF_RenderUTF8_Blended(font, screenText.c_str(), sty.fontColor);
				tempRect = {0, 0, sty.editc[2]-4*sty.border, sty.editc[3]-4*sty.border};
				SDL_BlitSurface(tempSurface, &tempRect, editSurface, &editRect);
				SDL_FreeSurface(tempSurface);
			SDL_BlitSurface(editSurface, &editRect, screen, &editRect);
			SDL_FreeSurface(editSurface);
			//=== side surface
			sideSurface = SDL_CreateRGBSurface(0, sty.winW,sty.winH, 32, 0,0,0,0);
			SDL_FillRect(sideSurface, &sideRect, SDL_MapRGBA(sideSurface->format,50,0,0,255));
			SDL_BlitSurface(sideSurface, &sideRect, screen, &sideRect);
			SDL_FreeSurface(sideSurface);
			//=== down surface
			downSurface = SDL_CreateRGBSurface(0, sty.winW,sty.winH, 32, 0,0,0,0);
			SDL_FillRect(downSurface, &downRect, SDL_MapRGBA(downSurface->format,0,0,50,255));
			SDL_BlitSurface(downSurface, &downRect, screen, &downRect);
			SDL_FreeSurface(downSurface);
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

	cv::Mat image = cv::imread("./ball.png");
	cv::Vec3b buf;

	ofstream outfile("./outtext.txt");
	if (!outfile) {
		return -1;
	}

	int p;
	for(int i=0; i<image.rows; i++) {
		for (int j=0; j<image.cols; j++) {
			buf = image.at<cv::Vec3b>(i,j);
			p = 0.299*(int)buf[2] + 0.587*(int)buf[1] + 0.114*(int)buf[0];
			string str = graypix(p);
			outfile << str;
			cout << str;
		}
		outfile << std::endl;
		cout << std::endl;
	}
	outfile.close();

	return 0;
}