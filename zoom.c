/*Write a program that will allow you to zoom into a selected area of a given image with
a specified zoom factor. The zoom factor is specified as an integer in the range
[1; max(r; c)] where r and c are the number of rows and number of columns in the image,
respectively. Zoom is to be achieved by pixel replication starting at the specified point
(center of zoom) and growing outwards, within a specified window. The area outside of zoom
window should be left undisturbed. Let us illustrate the command as follows, on a 640x480
image, named image.jpg.
zoom -z 5 -R 200 -C 300 -H 100 -W 100 image.jpg out.jpg
The options R and C specify the center of zoom window and H and W specify the height and
width of zoom window. Thus, the coordinates of zoom window are (150; 250) for bottom left
and (250; 350) for top right (specified as (r; c)).
The algorithm will proceed as follows. Replace the pixels in the 5x5 block between (200; 296)
and (204; 300) with the pixel at (200; 300). Replace the pixels in the block to the left of
this block by the pixel at (200; 299). And so on. Effectively, you will replace the pixels
with the specified window by replicating the pixels as needed.
Alternatively, you can implement this by using the mouse and trackbar callbacks instead of
specifying the command line parameters.*/


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
int z, R, C, H, W;
string originalImg = "", outImg = "";
Mat img, imgCopy;
const string DISPLAY = "Solution";

//function declaration
int performZoom();
void setupVariable(int, char**);
void on_trackbar(int, void*);
void mouse_LClick(int, int, int, int, void*);

int main(int argc, char ** argv) {
	//verify the argument for picture
	if (!(argc < 12)) {
		setupVariable(argc, argv);
	}
	else {
		originalImg = argv[1];
	}
	//read image
	img = imread(originalImg, CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty()) {
		cerr << "Unable to open picture file " << originalImg << endl;
		return (1);
	}
	// verify the zoom factor
	int max_z = ((img.rows > img.cols) ? img.rows : img.cols);
	if (z > max_z) {
		cerr << "Usage: Zoom factor z should be in Range (1,max(R,C))" << endl;
		return (1);
	}
	//make a clone and display the original image.
	imgCopy = img.clone();
	namedWindow(DISPLAY);
	imshow(DISPLAY, img);
	waitKey(1);
	
	if (!(argc < 12)) {
		performZoom();
	}
	
	//create trackbar for zoom, height and width
	createTrackbar("Zoom Factor", DISPLAY, &z, max_z, on_trackbar);
	createTrackbar("Zoom Height", DISPLAY, &H, img.rows, on_trackbar);
	createTrackbar("Zoom Width", DISPLAY, &W, img.cols, on_trackbar);
	//set the mouse callback function here.
	setMouseCallback(DISPLAY, mouse_LClick, &img);

	waitKey(0);

	destroyAllWindows();

	return (0);
}

void on_trackbar(int, void*) {
	imgCopy = img.clone();
	//performZoom();
}

void mouse_LClick(int event, int x, int y, int flags, void* param) {
	//on mouse click replace the R and C coordinates.
	imgCopy = img.clone();
	if (event == CV_EVENT_LBUTTONDOWN) {
		R = x;
		C = y;
		performZoom();
	}
}

//function to perform the zoom operation
int performZoom() {
	//set the initial points
	Point topLeft, bottomRight;
	topLeft.x = (R - H / 2);
	topLeft.y = (C - W / 2);
	bottomRight.x = (R + H / 2);
	bottomRight.y = (C + W / 2);
	//check the range 
	if (topLeft.x<0 || topLeft.y<0 || bottomRight.x>img.rows || bottomRight.y>img.cols)
	{
		cerr << "Out of range values selected." << endl;
		return (1);
	}
	//perform zoom 
	Mat newImage = Mat(img.rows*z, img.cols*z, CV_8UC1);
	for (int i = 0; i < z*img.rows; i++) {
		for (int j = 0; j < z*img.cols; j++) {
			newImage.at<uchar>(i*z*img.rows + j) = img.at<uchar>((i / z)*img.rows + (j / z));
		}
	}

	cout << "topLeft(x,y): " << topLeft << endl;
	cout << "bottomRight(x,y): " << bottomRight << endl;

	Point _center;
	_center.x = R*z;
	_center.y = C*z;
	//replace the zoomed pixel
	Rect tempRect((_center.x - H / 2), (_center.y - W / 2), H, W);
	Rect rect(topLeft.x, topLeft.y, H, W);
	Mat roi = newImage(tempRect);
	roi.copyTo(imgCopy(rect));

	imshow(DISPLAY, imgCopy);

}

void setupVariable(int argc, char ** argv) {
	//Extract variables of the command line flags
	string str = "";
	for (int i = 1; i < argc; i++) {
		str = argv[i];
		if (i + 1 != argc) {
			if (str == "-z") {
				z = atoi(argv[i + 1]);
			}
			else if (str == "-R") {
				R = atoi(argv[i + 1]);
			}
			else if (str == "-C") {
				C = atoi(argv[i + 1]);
			}
			else if (str == "-H") {
				H = atoi(argv[i + 1]);
			}
			else if (str == "-W") {
				W = atoi(argv[i + 1]);
			}
		}
	}
	originalImg = argv[11];
	outImg = argv[12];
}

