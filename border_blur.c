/*
Write a program to blur the borders of an image by making them pastel. 
The border should be such that it is all white at the edge and starts to 
fade n pixels from the border where n is a user specified parameter. The 
fading should happend in a linear fashion. If the pixel is valued at p, 
the pastel value of pixel at a distance d from border will be specified as
p +((n-d)/n)(MaxRGB-p)
Specify n using a trackbar, making sure that n < min(h;w) where h and w are 
the height and width of the image, respectively.
*/

//include headers
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//namespace declarations
using namespace cv;
using namespace std;

//global variables
const string DISPLAY = "Solution";
const string TRACKBAR_NAME = "Border Blur";
int MaxIntensity=256;
int n,n_max;
Mat img;
Mat imgclone;

//function declarations
void blurBorder(Mat&, double);
void on_trackbar(int, void*);

int main(int argc, char ** argv) {
	//check the arguments are not empty
	if (argc == 1) {
		cerr << "usage: " << argv[0] << " picture_file" << endl;
		return (1);
	}
	//load the image in the Mat container
	img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		cerr << "Unable to open picture file " << argv[1] << endl;
		return (1);
	}

	namedWindow(DISPLAY);
	imshow(DISPLAY, img);
	waitKey(1);

	n_max = (img.rows < img.cols) ? img.rows : img.cols;
	n = 0;
	createTrackbar(TRACKBAR_NAME, DISPLAY, &n, n_max, on_trackbar);
	on_trackbar(n, 0);

	waitKey(0);

	destroyAllWindows();

	return (0);
}

void on_trackbar(int, void*) {
	//make a copy of the image
	imgclone = img.clone();
	blurBorder(imgclone, n);
	imshow(DISPLAY, imgclone);
	waitKey(1);
}

void blurBorder(Mat& img, double n) {
	int i, j;
	double d, p, pastelValue;
	double top = n, left = n, right = img.cols - n, bottom = img.rows - n;
	//define border for horizontal axis
	for (i = 0; i<img.rows; i++) {
		for (j = 0; j<img.cols; j++) {
			p = img.at<uchar>(i, j);
			pastelValue = p;
			if (i<top) {
				d = i;
				pastelValue = p + (n - d) / n*(MaxIntensity - p);
			}
			else if (i>bottom) {
				d = img.rows - i - 1;
				pastelValue = p + (n - d) / n*(MaxIntensity - p);
			}
			img.at<uchar>(i, j) = pastelValue;
		}
	}
	//define border for vertical axis
	for (i = 0; i<img.rows; i++) {
		for (j = 0; j<img.cols; j++) {
			p = img.at<uchar>(i, j);
			pastelValue = p;
			if (j<left) {
				d = j;
				pastelValue = p + (n - d) / n*(MaxIntensity - p);
			}
			else if (j>right) {
				d = img.cols - j - 1;
				pastelValue = p + (n - d) / n*(MaxIntensity - p);
			}
			img.at<uchar>(i, j) = pastelValue;
		}
	}

}
