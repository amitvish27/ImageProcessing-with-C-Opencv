/*Determine statistics on an image. Find the average value of each of RGB and HSI components in the image. Report the
H component in degrees

	@author Avishwakarma
	11/28/16
*/


//header includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//global variables
double const PI = 3.14159265;
double sum_R, sum_G, sum_B, sum_H, sum_S, sum_I, pxCount;

//method identifiers
void displayStats();
void calcStats(Mat&);

int main(int argc, char** argv)
{
	//check the number of arguments
	if (argc != 2)
	{
		cout << " Usage: imgStats image" << endl;
		return -1;
	}

	Mat image;
	// Read the file
	image = imread(argv[1], IMREAD_COLOR);

	// Check for invalid input
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	//total number of pixels in image
	pxCount = image.rows*image.cols;

	//calculate statistics
	calcStats(image);
	//display statistics
	displayStats();

	//Display Original Image
	imshow("RGB Image", image);

	//wait 'til key detected
	waitKey(0);
	destroyAllWindows(); //destroy all open windows
	return 0;
}

void displayStats() {
	double avg = 0.0;
	//Display all the image component average
	cout << "Image Statistics:\nB: ";
	avg = sum_B / pxCount;
	cout << (avg * 255);

	cout << "\nG: ";
	avg = sum_G / pxCount;
	cout << (avg * 255);

	cout << "\nR: ";
	avg = sum_R / pxCount;
	cout << (avg * 255);

	cout << "\nH in degrees: ";
	avg = sum_H / pxCount;
	cout << (avg * 180 / PI);

	cout << "\nS (range 0 to 1): ";
	avg = sum_S / pxCount;
	cout << avg;

	cout << "\nI (range 0 to 255): ";
	avg = sum_I / pxCount;
	cout << (avg * 255);
}

void calcStats(Mat& image)
{
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			double R, G, B, H, S, I, Q, min_val, sum_val;

			//Get the BGR values
			B = image.at<Vec3b>(i, j)[0];
			G = image.at<Vec3b>(i, j)[1];
			R = image.at<Vec3b>(i, j)[2];

			//Normalize RGB in range 0-1
			sum_val = R + G + B;
			R = R / sum_val;
			G = G / sum_val;
			B = B / sum_val;

			//Calculate the intensity in range 0-1
			I = sum_val / (3.0 * 255);

			//Calculate the saturation
			min_val = (B < G) ? ((B < R) ? B : (R < G) ? R : G) : ((R < G) ? R : G);
			S = 1 - (3.0 * min_val);

			//Calculate the Hue 
			if (S != 0.0) {
				Q = acos((0.5*((R - G) + (R - B))) / sqrt((R - G)*(R - G) + (R - B)*(G - B)));

				if (B <= G) {
					H = Q; //H in range 0-180
				}
				else {
					H = 2 * PI - Q; //H in range 180-360;
				}
			}
			else {
				H = 0.0;
			}

			//calculate sum of the components
			sum_R += R;
			sum_G += G;
			sum_B += B;
			sum_H += H;
			sum_S += S;
			sum_I += I;
		}
	}
}