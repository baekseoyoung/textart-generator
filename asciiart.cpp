#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <gtk/gtk.h>
#include "asciicalculate.hpp"

int main(int argc, char **argv) {
	cv::Mat image = cv::imread("./ball.png");
	cv::Vec3b buf;

	std::ofstream outfile("./outtext.txt");

	int p;
	for(int i=0; i<image.rows; i++) {
		for (int j=0; j<image.cols; j++) {
			buf = image.at<cv::Vec3b>(i,j);
			p = 0.299*(int)buf[2] + 0.587*(int)buf[1] + 0.114*(int)buf[0];
			std::string str = graypix(p);
			outfile << str;
			std::cout << str;
		}
		outfile << std::endl;
		std::cout << std::endl;
	}
	outfile.close();

	return 0;
}