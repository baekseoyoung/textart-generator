#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

// enum MouseState
// {
// 	MOUSE_ON_EDITBOX,
// 	MOUSE_ON_SIDEBOX,
// 	MOUSE_ON_DOWNBOX
// };

void func_keyboard_event(SDL_Event e) {
	if(SDL_GetModState() & KMOD_CTRL) { //=== ctrl
		switch(e.key.keysym.sym) {
			case SDLK_s:
				cout<<"saved"<<endl;
				break;
		}
	} else {
		switch(e.key.keysym.sym) { //=== text input related
			case SDLK_BACKSPACE:
				cout<<"delete txt"<<endl;
				break;
			case SDLK_RETURN:
				cout<<"line break"<<endl;
				break;
		}
	}
}

void func_mouse_event(SDL_Event e) {
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		cout << x << " " << y << " down ";
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		cout << x << " " << y << " up ";
	}
}
