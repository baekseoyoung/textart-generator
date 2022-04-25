#include <iostream>
#include <string>
#include <vector>

#include "asciicalculate.hpp"

using namespace std;

void func_init_text(int textRow, int textCol, vector<string> *text) {
	string temp;
	for(int i=0; i<textRow; i++) {
		temp = "";
		for(int j=0; j<textCol; j++) {
			temp += " ";
		}
		(*text).push_back(temp);
	}
}

string func_gray2text(int p) {
	string str;
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
