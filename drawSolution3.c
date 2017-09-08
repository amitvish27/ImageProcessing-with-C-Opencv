/*
 * drawSolution3.c
 *
 *  Created on: Sep 23, 2016
 *      Author: Ameet Vishwakarma
 *
 *      Problem: Use OpenCV to create an 8-bit single-channel image of size
 *      640x480 pixels. In this image, at random locations, place a square of
 *      size 100x100 pixels, a rectangle of size 200x160 pixels, and a circle
 *      of diameter 70 pixels. Your background should be black. For each object,
 *      the center pixel of the object will be white. Other pixels will be
 *      given a grayscale value as 0XFF - d where d is the distance of the pixel
 *      from the center. Make sure that you crop the objects at the borders. If
 *      two objects overlap, the object being drawn later takes precedence. Call
 *      your executable objects. Make sure you display the image on screen.
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

const char * DISPLAY = "Solution3";
const int width = 640;
const int height = 480;

int randX();
int randY();
void drawRectangle(Mat&, Point, Point);
void drawCircle(Mat&, Point, int);
double eDistance(double d1, double d2);

int main() {
	int x, y;
	//initialize random seed
	srand(time(NULL));
	Mat img;
	img = Mat::zeros(Size(width, height), CV_8UC1);
	if (img.empty()) {
		cerr << "Unable to open Display" << endl;
		return (1);
	}

	//Generate random point for rectangle
	x = randX();
	y = randY();
	cout << "Rectangle at Point(x,y) - " << x << "," << y << endl;
	//draw rectangle starting at Pint(x,y) with width=200 and height=160
	drawRectangle(img, Point(x, y), Point(x + 200, y + 160));

	//Generate random point for square starting at Point(x,y) with sides 100
	x = randX();
	y = randY();
	cout << "Square at Point(x,y) - " << x << "," << y << endl;
	drawRectangle(img, Point(x, y), Point(x + 100, y + 100));

	//Generate random point for Circle staring at Point(x,y) with diameter 70
	x = randX();
	y = randY();
	cout << "Circle at Point(x,y) - " << x << "," << y << endl;
	drawCircle(img, Point(x, y), 70);

	namedWindow(DISPLAY);
	imshow(DISPLAY, img);
	waitKey(0);

	return (0);
}

int randX() {
	return rand() % height + 1;
}

int randY() {
	return rand() % width + 1;
}

void drawRectangle(Mat& img, Point pt1, Point pt2) {
	//calculate the height and width from the given coordinates
	int h = (pt1.x < pt2.x) ? (pt2.x - pt1.x) : (pt1.x - pt2.x);
	int w = (pt1.y < pt2.y) ? (pt2.y - pt1.y) : (pt1.y - pt2.y);
	cout << "width x height of rectangle:" << w << "x" << h << endl;

	//calculate the center points
	int center_x = pt1.x + h / 2;
	int center_y = pt1.y + w / 2;
	cout << "center points at p(" << center_x << "," << center_y << ")" << endl;

	double max_dist, curr_dist;

	//Calculate largest distance from center
	max_dist = eDistance((pt1.x - center_x), (pt1.y - center_y));

	for (int i = pt1.x; i < pt2.x; i++) {
		for (int j = pt1.y; j < pt2.y; j++) {

			if (!(i > height || j > width)) {
				curr_dist = eDistance((i - center_x), (j - center_y));
				img.at<uchar>(i, j) = 0xff - (curr_dist / max_dist) * 0xff;
//				cout<<(curr_dist / max_dist)*255<<endl;
			}
		}
	}
}

void drawCircle(Mat& img, Point pt1, int diameter) {
	//calculate the radius from the given diameter
	int radius = diameter / 2;
	cout << "radius of circle : " << radius << endl;

	//calculate the center points
	int center_x = pt1.x + radius;
	int center_y = pt1.y + radius;
	cout << "center points at p(" << center_x << "," << center_y << ")" << endl;

	double max_dist, curr_dist;

	//Calculate largest distance from center
	max_dist = eDistance((pt1.x - center_x), (pt1.y - center_y));

	//equation of circle r >= sqrt(x2+y2)

	for (int i = pt1.x; i < pt1.x + diameter; i++) {
		for (int j = pt1.y; j < pt1.y + diameter; j++) {

			if (!(i > height || j > width)) {
				curr_dist = eDistance((i - center_x), (j - center_y));
				if (curr_dist <= radius)
					img.at<uchar>(i, j) = 0xff - (curr_dist / max_dist) * 0xff;
			}
		}
	}
}

double eDistance(double d1, double d2) {
	return sqrt(d1 * d1 + d2 * d2);
}

