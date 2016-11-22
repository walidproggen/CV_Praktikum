#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "myFunctions.h"
#include "histogram.h"

using namespace cv;
using namespace std;


extern bool selectObject;
extern Mat image;
extern Mat img_untouched;
extern Mat roi;
extern Mat frame;
extern Rect selection;
extern int *hBin, *sBin, *vBin;


int main (int argc, char* argv[]) {

	if (argc != 2) { cout << "Usage: Programname + (A5 || A6 || A7)" << endl; return 0; }
	String str = argv[1];
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
//+++++++++++++++++++++++++++++++++++++++++++++++ Praktikum 3 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
	if (str == "A6" || (str == "A7")) {
	 	VideoCapture cap("http://192.168.0.10:4747/mjpegfeed?x.mjpeg");
	 	namedWindow( "Webcam", WINDOW_AUTOSIZE );
		String userdata = "Ungleich Null";
	 	setMouseCallback( "Webcam", onMouseCallBack, &userdata );
		createTrackbar("Hue", "Webcam", hBin, 180, onTrackBar);
		createTrackbar("Saturation", "Webcam", sBin, 256, onTrackBar);
		createTrackbar("Value", "Webcam", vBin, 256, onTrackBar);

		for (;;) {
			cap >> frame;
			if (frame.empty()) { break; }
			if( selectObject && selection.width > 0 && selection.height > 0 ) {
				//histogram2D (roi);
				trackObject ();
			}
	  		imshow( "Webcam", frame );
			if ((waitKey(10) % 256) == 27) { break; }
		}
	} 


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
//+++++++++++++++++++++++++++++++++++++++++++++++ Praktikum 2 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
	else if (str == "A5" || str == "A4") {
		// read image file
		image = imread ("full.png", 1);
		if (! image.data) {
			cout << "Could not open or find the image" << endl;
			return -1;
		}
		image.copyTo(img_untouched);

		// create window, display image in window and set callback
		namedWindow ("Bild in Farbe", WINDOW_AUTOSIZE); // Create a windows for display
		setMouseCallback("Bild in Farbe", onMouseCallBack, NULL);	//set mouse callback for specified window
		imshow ("Bild in Farbe", image); // Show our image inside it

		waitKey(0);
	}

	return(0);
}
