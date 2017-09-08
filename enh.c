/*Write a program to perform enhancement shown in Figure 3-38 of your textbook (Laplacian
method). Call the executable enh.
Run your code on the image hoare:~sanjiv/cs5420/assignment4/agblur.jpg.*/


//Header includes
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string> 

//namespace declaration
using namespace cv;
using namespace std;

//global variables
Mat img, laplacianImg;
const string DISPLAY = "Solution";
int k_size = 3;
int padding;
int maskFlag = 1;
int laplacian1[3][3] = { { 0,1,0 },{ 1,-4,1 },{ 0,1,0 } };
int laplacian2[3][3] = { { 1,1,1 },{ 1,-8,1 },{ 1,1,1 } };

//function declarations
void applyFilter();
void displayKernel(int[3][3]);

int main(int argc, char ** argv) {

	if (argc == 1) {
		cerr << "usage: " << argv[0] << " picture_file" << endl;
		return (1);
	}

	//read image
	img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		cerr << "Unable to open picture file " << argv[1] << endl;
		return (1);
	}

	cout << "select from the following masks: \n1.";
	displayKernel(laplacian1);
	cout << "2.";
	displayKernel(laplacian2);

	cin >> maskFlag;

	namedWindow(DISPLAY);
	applyFilter();
	//namedWindow("Original");
	//imshow ("Original",img) ;
	waitKey(0);
	destroyAllWindows();
	return (0);
}

void applyFilter() {
	laplacianImg = Mat(img.rows,img.cols,CV_8UC1);	

	int newPx;
	for (int i = 1; i < img.rows - 1; i++) {
		for (int j = 1; j < img.cols - 1; j++) {
			//apply the kernel here to each pixel
			newPx = img.at<uchar>(i + 1, j) + img.at<uchar>(i - 1, j) +
				img.at<uchar>(i, j + 1) + img.at<uchar>(i, j - 1) + img.at<uchar>(i, j) * (-4);
			if (maskFlag == 2) {
				newPx += img.at<uchar>(i + 1, j + 1) + img.at<uchar>(i - 1, j - 1) +
					img.at<uchar>(i - 1, j + 1) + img.at<uchar>(i + 1, j - 1) + img.at<uchar>(i, j) * (-4);
			}

			if (newPx < 0) { newPx = 0; }
			if (newPx > 255) { newPx = 255; }

			laplacianImg.at<uchar>(i, j) = newPx;
		}
	}
	
	Mat outImg = img + laplacianImg;
	
	for (int i = 0; i < outImg.rows; i++) {
		for (int j = 0; j < outImg.cols; j++) {
			newPx = (int)outImg.at<uchar>(i, j);
			if (newPx < 0) { newPx = 0; }
			if (newPx > 255) { newPx = 255; }
			outImg.at<uchar>(i, j) = newPx;
		}
	}
	imshow(DISPLAY, outImg);
}

void displayKernel(int temp[3][3]) {
	int i, j;
	cout << "k:";
	for (i = 0; i < k_size; i++) {
		for (j = 0; j < k_size; j++) {
			cout << "\t" << temp[i][j];
		}
		cout << endl;
	}
}
