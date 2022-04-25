#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <windows.h>

#include "asciicalculate.hpp"

using namespace std;

void func_read_file(char* path, vector<string> *text) {
	string filePath(path);
	string fileExten = filePath.substr(filePath.find_last_of(".")+1);
	if (fileExten == "txt") {
		// cout<<"text!"<<endl;
		ifstream infile(filePath);
		string line;
		int i = 0;
		(*text).clear();
		while(getline(infile, line)) {
			(*text).push_back(line);
		}
		infile.close();
	} else if (fileExten=="png" || fileExten=="jpg" || fileExten=="jpeg") {
		// cout<<"img!"<<endl;
		cv::Mat img = cv::imread(filePath);
		cv::Vec3b buf;
		int pixel;
		string line;
		(*text).clear();
		for(int i=0; i<img.rows; i++) {
			line = "";
			for (int j=0; j<img.cols; j++) {
				buf = img.at<cv::Vec3b>(i,j);
				pixel = 0.299*(int)buf[2] + 0.587*(int)buf[1] + 0.114*(int)buf[0];
				line = line + func_gray2text(pixel);
			}
			(*text).push_back(line);
		}
	}
}
void func_write_file(vector<string> *text) {
	ofstream outfile("./outtext.txt");
	if (outfile) {
		for(int i=0; i<(int)sizeof(*text); i++) {
			outfile<<(*text)[i]<<endl;
		}
		outfile.close();
	}
}
void func_win_open_dialog(vector<string> *text) {
	OPENFILENAME ofn = {0}; //== common dialog box structure. set to 0 important.
	char fileName[200] = {0}; //== file name buffer
	HWND hwnd = NULL; //== owner window
	// HANDLE hf; //== file handle

	//=== initialize rest of the fields of ofn.
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0'; //== so GetOpenFileName does not use the content of fileName to initialize itself.
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = "All files\0*.*\0Text files\0*.TXT\0Images(png, jpg, jpeg)\0*.png\0*.jpg\0*.jpeg\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//=== display
	if(GetOpenFileName(&ofn) == true) {
		func_read_file(ofn.lpstrFile, text);
		// hf = CreateFile(
		// 	ofn.lpstrFile,
		// 	GENERIC_READ,
		// 	0,
		// 	(LPSECURITY_ATTRIBUTES) NULL,
		// 	OPEN_EXISTING,
		// 	FILE_ATTRIBUTE_NORMAL,
		// 	(HANDLE) NULL);
	}
}
