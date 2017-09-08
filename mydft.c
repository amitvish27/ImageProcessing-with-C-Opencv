/*Write a frequency-domain ﬁlter to remove periodic noise from the picture noisy_backyard.jpg. You can use 
OpenCV Fourier transform. For an example, 
check http://docs.opencv.org/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
	
	@author Avishwakarma
	12/3/16
*/


//Header Files
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//global variables
Mat image, spectDFT, magI, gaussianMask, outputImage;
int kernel_size = 11;
vector<Point> _mpoints;

//function identifiers
void computeDFT();
void applyGLPF();
void applyIDFT();
void displaySpectrum(Mat complex);
void rearrangeQuadrants(Mat temp);
void onMouse(int event, int x, int y, int, void* param);

int main(int argc, char** argv){

	//check the number of arguments
	if (argc != 2){
		cout << " Usage: mydft image" << endl;
		return -1;
	}
	
	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	
	// Check for valid input only
	if (!image.data) {
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Show our image inside it.
	imshow("Orginal window", image);                   
	
	//compute the DFT of the image
	computeDFT();
	
	//create empty gaussian mask for points to plot
	gaussianMask = Mat::zeros(Size(spectDFT.cols, spectDFT.rows), CV_32F);
	
	setMouseCallback("DFT Spectrum", onMouse, (void*)&_mpoints);
	createTrackbar("kernelSize", "DFT Spectrum", &kernel_size, 127);

	waitKey(0);
	destroyAllWindows();
	return 0;
}

void onMouse(int event, int x, int y, int, void* param){

	if (event == CV_EVENT_LBUTTONDOWN) {
		//get coordinate points to spectrum
		vector<Point>* ptPtr = (vector<Point>*)param;
		ptPtr->push_back(Point(x, y));
		//for display purpose only
		circle(magI, Point(x, y), kernel_size / 2, Scalar(0), -1);
		//for the actual gaussian mask 
		circle(gaussianMask, Point(x, y), kernel_size / 2, Scalar(255), -1);
		//display the magnitude of spectrum with new points plotted
		imshow("DFT Spectrum", magI);
	}
	if (event == CV_EVENT_RBUTTONDOWN) {
		//Apply the Gaussian Low pass filter to it
		applyGLPF();
		
		//Apply the Inverse DFT to the new image
		applyIDFT();
	}
	return;
}

void applyIDFT()
{
	//inverse DFT
	idft(spectDFT, outputImage);

	// Split planes planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	Mat planes[] = { Mat::zeros(spectDFT.size(), CV_32F), Mat::zeros(spectDFT.size(), CV_32F) };
	split(outputImage, planes);

	// Calculate magnitude and store in planes[0] = magnitude
	magnitude(planes[0], planes[1], outputImage);

	//Normalize the inverse spectrum
	normalize(outputImage, outputImage, 0, 1, NORM_MINMAX);

	//Dsiplay the result
	imshow("result", outputImage);
}

void applyGLPF() {
	//Mat mask = createGausianFilter();
	normalize(gaussianMask, gaussianMask, 0, 1, NORM_MINMAX);

	//invert the mask
	gaussianMask = Mat::ones(gaussianMask.size(), CV_32F) - gaussianMask;
	//imshow("mask", gaussianMask);
	rearrangeQuadrants(gaussianMask);

	Mat planes[] = { Mat::zeros(spectDFT.size(), CV_32F), Mat::zeros(spectDFT.size(), CV_32F) };

	Mat k_mask;
	planes[0] = gaussianMask; // real
	planes[1] = gaussianMask; // imaginary
	merge(planes, 2, k_mask); //get the real part to kernel_mask*/*/

	//multiply Gaussian Mask with DFT Spectrum
	mulSpectrums(spectDFT, k_mask, spectDFT, DFT_ROWS); //only DFT_ROWS flag is accepted
	displaySpectrum(spectDFT);
}

void computeDFT() {
	Mat padded;
	//expand input image to optimal size
	int m = getOptimalDFTSize(image.rows);
	int n = getOptimalDFTSize(image.cols);
	// on the border add zero values
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	// Add to the expanded another plane with zeros
	merge(planes, 2, spectDFT);
	// this way the result may fit in the source matrix
	dft(spectDFT, spectDFT);
	displaySpectrum(spectDFT);
}


void displaySpectrum(Mat complex)
{
	//---------------- To Display the DFT Spectrum-----------------//
	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	Mat planes[] = { Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F) };
	split(complex, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

	magnitude(planes[0], planes[1], magI);    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

											  // switch to logarithmic scale: log(1 + magnitude)
	magI += Scalar::all(1);
	log(magI, magI);
	rearrangeQuadrants(magI);
	normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
										   // viewable image form (float between values 0 and 1).
	imshow("DFT Spectrum", magI);
}

void rearrangeQuadrants(Mat temp) {
	// crop the spectrum, if it has an odd number of rows or columns
	temp = temp(Rect(0, 0, temp.cols & -2, temp.rows & -2));

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = temp.cols / 2;
	int cy = temp.rows / 2;

	Mat q0(temp, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(temp, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(temp, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(temp, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}