/*Write a program to generate a circular kernel of specified size and resolution. The resolution will
be specified as a power of 2. Thus, if the resolution is 1024, the coefficients of kernel will add to
1024, with each coefficient weighted by its distance from center. Obviously, the center pixel will have
the highest weight, and corner pixels will have low weight, possibly 0. Apply this kernel to blur an
image. You will actually be better off writing a convolution function that can apply the kernel to the
image in this problem as well as the problem above. Take care of the edges by using reflected pixels.
Call the executable blur.*/

//include headers
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

//namespace declarations
using namespace cv;
using namespace std;

//global variables
const string DISPLAY = "Solution";
int padding, k_size = 0, k_resolution = 1, sigma = 1;
const double pi = 3.141592;
Mat img, imgCopy;
Vector< Vector<double> > _ckernel;

//function declarations
void performBlur();
void cKernel();
double eDistance(double d1, double d2);

int main(int argc, char ** argv) {
	//check the arguments are not empty
	if (argc < 4) {
		cerr << "usage: " << argv[0] << " picture_file kernel_size kernel_resolution" << endl;
		return (1);
	}
	//load the image in the Mat container
	img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		cerr << "Unable to open picture file " << argv[1] << endl;
		return (1);
	}

	k_size = atoi(argv[2]);
	k_size = (k_size % 2 == 0) ? (k_size + 1) : k_size; // make sure kernel size is always odd
	padding = k_size / 2; // generate padding rate

	k_resolution = atoi(argv[3]);

	imgCopy = img.clone();

	performBlur();

	namedWindow(DISPLAY);
	imshow(DISPLAY, imgCopy);
	namedWindow("Original");
	imshow("Original", img);

	waitKey(0);

	destroyAllWindows();

	return (0);
}

void performBlur() {
	cKernel();
	int newpxVal;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			//for kernel
			for (int ki = -padding; ki <= padding; ki++) {
				for (int kj = -padding; kj <= padding; kj++) {
					//by using reflected pixels on edges;
					int x = i - ki, y = j - kj;
					if (x < 0) {
						x = -i - padding;
					}
					else if (x >= img.rows) {
						x = 2 * img.rows - i - padding;
					}

					if (y < 0) {
						y = -j - padding;
					}
					else if (y >= img.cols) {
						y = 2 * img.cols - i - padding;
					}

					newpxVal += (int) (_ckernel[ki + padding][kj + padding] * img.at<uchar>(x, y)); //apply kernel to image 
				}
			}
			newpxVal /= (k_resolution);

			imgCopy.at<uchar>(i, j) = newpxVal;
		}
	}
}


//using the circle
void cKernel() {
	double curr_dist, sum = 0.0;
	//equation of circle r >= sqrt(x2+y2)
	int radius = k_size / 2;

	//calculate the circular kernel
	_ckernel.resize(k_size);
	for (int i = 0; i < k_size; i++) {
		_ckernel[i].resize(k_size);
		for (int j = 0; j < k_size; j++) {
			curr_dist = eDistance((i - radius), (j - radius));
			if (curr_dist <= k_size) {
				_ckernel[i][j] = (k_size - curr_dist) - radius; //think
			}
			else {
				_ckernel[i][j] = 0;
			}

			if (_ckernel[i][j] < 0) {
				_ckernel[i][j] = 0;
			}

			sum += _ckernel[i][j];
		}
	}
	//apply resolution to the kernel
	for (int i = 0; i < k_size; i++) {
		for (int j = 0; j < k_size; j++) {
			_ckernel[i][j] = _ckernel[i][j] * (k_resolution / sum);
			//cout << _ckernel[i][j] << "\t" ;  
		}
		//cout <<endl;
	}

}

double eDistance(double d1, double d2) {
	return sqrt(d1 * d1 + d2 * d2);
}

/*void cKernel() {
float radius = k_size / 2.0;
cout << "radius " << radius << endl;
float x1;
_ckernel.resize(radius);
for (float y1 = 0; y1 < radius; y1 = y1 + 0.25) {
_ckernel[y1].resize(radius);
x1 = radius - sqrt(radius*radius - (radius - y1)*(radius - y1));
_ckernel[x1][y1] = 1;
cout << _ckernel[x1][y1] << endl;
}
}*/
