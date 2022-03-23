#include <iostream>
#include <string>
#include "asciicalculate.hpp"

std::string graypix(int p) {
	std::string str;
	if (p > 250) {
		str = " ";
	} else if (p > 200) {
		str = " ";
	} else if (p > 150) {
		str = ".";
	} else if (p > 100) {
		str = "*";
	} else if (p > 50) {
		str = "/";
	} else {
		str = "M";
	}
	return str;
}