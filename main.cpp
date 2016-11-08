#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "myFunctions.h"
#include "histogram.h"

using namespace cv;
using namespace std;

Mat image, img_untouched, roi;
Point mouseDownCoor;	// stores the coordinates of mouse DOWN event


void myCallBackFunc (int event, int x, int y, int flags, void* userdata) {
	// Event occur when left mouse button is down 
	if (event == EVENT_LBUTTONDOWN) { 
		cout << "Left button is clicked in position x: " << x << "and y: " << y << endl; 
		mouseDownCoor.x = x; 
		mouseDownCoor.y = y; 
	}
	
	// Event occur when left mouse button is released
	if (event == EVENT_LBUTTONUP) { 
		// when mouse was not moved and just was clicked (i.e. no rectangle was specified), restore origin picture 
		if (mouseDownCoor.x == x && mouseDownCoor.y == y) { 
			imshow ("Bild in Farbe", img_untouched); 
			img_untouched.copyTo(image); 
			destroyWindow("Submatrix");
			destroyWindow("histogram");
			return; 
		}
		// else draw a (white) rectangle in specified range
		cout << "Left button released at position x: " << x << "and y: " << y << endl;
		rectangle (image, mouseDownCoor, Point(x,y), Scalar(255, 255, 255)); 
		imshow ("Bild in Farbe", image);

/*		// when x or y coordinate exceed picture/window size, readjust them
		Size size = img_untouched.size();
		x = x<0 ? 0:x; x = x<size.width ? x:size.width;
		y = y<0 ? 0:y; y = y<size.height ? y:size.height;
		
		// save mouse coordinates into a range: rangeWidth(x1, x2), rangeHeight(y1,y2)
		Range rangeWidth(mouseDownCoor.x > x ? x:mouseDownCoor.x, mouseDownCoor.x > x ? mouseDownCoor.x:x);
		Range rangeHeight(mouseDownCoor.y > y ? y:mouseDownCoor.y, mouseDownCoor.y > y ? mouseDownCoor.y:y);

		cout << "rangeWidth.start = " << rangeWidth.start << "; rangeWidth.end = " << rangeWidth.end << endl;
		cout << "rangeHeight.start = " << rangeHeight.start << "; rangeHeight.end = " << rangeHeight.end << endl;

		cout << "rangeWidth.size() = " << rangeWidth.size() << endl; 
		cout << "rangeHeight.size() = " << rangeHeight.size() << endl; */

		Rect r = computeROI(mouseDownCoor, Point(x,y), img_untouched.size());

		// create submatrix and display the subimage
		roi = img_untouched(r).clone();
		namedWindow ("Submatrix", WINDOW_AUTOSIZE);
		imshow ("Submatrix", roi);

		// create histogram of submatrix
		computeHistogram(roi);
	}
}

int main () {
	// read image file
	image = imread ("full.png", 1);
	if (! image.data) {
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	image.copyTo(img_untouched);

	// create window, display image in window and set callback
	namedWindow ("Bild in Farbe", WINDOW_AUTOSIZE); // Create a windows for display
	setMouseCallback("Bild in Farbe", myCallBackFunc, NULL);	//set mouse callback for specified window
	imshow ("Bild in Farbe", image); // Show our image inside it

	waitKey(0);
	return(0);
}
