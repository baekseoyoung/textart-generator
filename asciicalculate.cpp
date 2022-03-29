#include <iostream>
#include <string>
#include "asciicalculate.hpp"

std::string graypix(int p) {
	std::string str;
	if (p > 250) {
		str = "  ";
	} else if (p > 200) {
		str = "  ";
	} else if (p > 150) {
		str = "..";
	} else if (p > 100) {
		str = "**";
	} else if (p > 50) {
		str = "//";
	} else {
		str = "MM";
	}
	return str;
}

std::string func_init_pixel(int txtW, int txtH) {
	std::string outTxt = "";
	for(int i=0; i<txtH; i++) {
		for(int j=0; j<txtW; j++) {
			outTxt += " ";
		}
		outTxt += "\n";
	}
	return outTxt;
}