#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

string func_sdl_keyboard_event(SDL_Event e, string inText)
{
	if (SDL_GetModState() & KMOD_CTRL) {
		switch(e.key.keysym.sym) {
			case SDLK_s: //===ctrl+s
				cout<<"saved!"<<endl;
				break;
			}
	} else {
		switch(e.key.keysym.sym) {
			case SDLK_BACKSPACE:
				if(inText.length() > 0) {
					if (inText.back() & 0x80) { //===if korean
						inText.pop_back();
						inText.pop_back();
					}
					inText.pop_back();
				}
				break;
			case SDLK_RETURN:
			inText += "\n";
			break;
			//default:
		}
	}
	return inText;
}

void func_sdl_mouse_event(SDL_Event e)
{

}