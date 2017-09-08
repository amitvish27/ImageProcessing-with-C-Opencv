/*Change the intensity and saturation of colors in an image using a trackbar for each of them

	@author Avishwakarma
	11/30/16
*/

//header includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//global variables
const string DISPLAY = "Solution";
int i_val = 0, s_val = 0, max_saturation = 100, max_intensity = 255;
double avgI, avgS;
const double PI = 3.14159265;
Mat image, imageClone, imageHSI;

//method identifiers
void changeIntensity(int, void*);
void changeSaturation(int, void*);
void modifyComponent();
void cvt_RGB_HSI(Mat& image, Mat& imageHSI);

int main(int argc, char** argv) {
	//check the number of arguments
	if (argc != 2) {
		cout << " Usage: sat_int_tb image" << endl;
		return -1;
	}
	// Read the file
	image = imread(argv[1], IMREAD_COLOR);
	// Check for invalid input
	if (!image.data) {
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	imageClone = image.clone();
	//Display Original Image
	namedWindow(DISPLAY, CV_WINDOW_AUTOSIZE);
	imshow(DISPLAY, image);

	//Get RGB to HSI image
	imageHSI = Mat(Size(imageClone.rows, imageClone.cols), CV_8UC3);;
	cvt_RGB_HSI(imageClone, imageHSI);
	i_val = avgI;
	s_val = avgS;
	//create trackbar for saturation and intensity
	createTrackbar("Intensity", DISPLAY, &i_val, max_intensity, changeIntensity);
	createTrackbar("Saturation", DISPLAY, &s_val, max_saturation, changeSaturation);

	//wait 'til key is detected
	waitKey(0);
	//destroy all open windows
	destroyAllWindows();
	return 0;
}

void changeIntensity(int, void*) {
	modifyComponent();
}
void changeSaturation(int, void*) {
	modifyComponent();
}
void modifyComponent() {
	imageClone = image.clone();
	//imshow("HSI image", imageHSI);

	for (int i = 0; i < imageClone.rows; i++)
	{
		for (int j = 0; j < imageClone.cols; j++)
		{
			double R, G, B, H, S, I;

			//Get the HSI values
			H = (double)imageHSI.at<Vec3b>(i, j)[0] * (PI / 180);
			S = (double)imageHSI.at<Vec3b>(i, j)[1] / 100.0;
			I = (double)imageHSI.at<Vec3b>(i, j)[2] / 255.0;

			//Inew = (1.0- (Iold + i_val/255))2
			I = (1.0 - (I - i_val / 255.0)) / 2;
			S = (1.0 - (S - s_val / 100.0)) / 2;

			//RG Sector 0 <= H < 120
			if (H >= 0 && H < 120) {
				B = I * (1 - S);
				R = I * (1 + ((S * cos(H)) / (cos((PI / 3) - H))));
				G = 3 * I - (B + R);
			}
			//GB Sector 120 <= H < 240 
			else if (H >= 120 && H < 240) {
				H = H - 120;
				R = I * (1 - S);
				G = I * (1 + ((S * cos(H)) / (cos((PI / 3) - H))));
				B = 3 * I - (R + G);
			}
			//BR Sector 240 <= H < 360
			else {
				H = H - 240;
				G = I * (1 - S);
				B = I * (1 + ((S * cos(H)) / (cos((PI / 3) - H))));
				R = 3 * I - (G + B);
			}

			//Denormalize the values
			B *= 255;
			G *= 255;
			R *= 255;

			//control overflow
			R = (R < 0) ? 0 : (R > 255 ? 255 : R);
			G = (G < 0) ? 0 : (G > 255 ? 255 : G);
			B = (B < 0) ? 0 : (B > 255 ? 255 : B);

			//insert the values of HSI in the modified Mat
			imageClone.at<Vec3b>(i, j)[0] = B;
			imageClone.at<Vec3b>(i, j)[1] = G;
			imageClone.at<Vec3b>(i, j)[2] = R;
		}
	}
	imshow(DISPLAY, imageClone);
}

void cvt_RGB_HSI(Mat& image, Mat& imageHSI)
{
	double sumI = 0.0, sumS = 0.0;
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

			//Denormalize the values
			H *= 180 / PI;
			S *= 100;
			I *= 255;
			sumI += I;
			sumS += S;
			//insert the values of HSI in the modified Mat
			imageHSI.at<Vec3b>(i, j)[0] = H;
			imageHSI.at<Vec3b>(i, j)[1] = S;
			imageHSI.at<Vec3b>(i, j)[2] = I;
		}
	}

	// to calculate the average of I & S
	avgI = sumI / (imageHSI.rows*imageHSI.cols);
	avgS = sumS / (imageHSI.rows*imageHSI.cols);

}
