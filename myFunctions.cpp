#include "myFunctions.h"
#include "histogram.h"


Rect selection;
Point origin;
Mat image, img_untouched;	// fuer Praktikum 2 
Mat hist, roi;
Mat frame, hsv_roi, hsv_frame, backProj;	// Fuer Praktikum 3
vector<Mat>* histoVec;
bool selectObject = false;

// Test different normalize functions
Mat backProj2;

// define ranges and channels for hist
float h_range[] = { 0, 180 };
float s_range[] = { 0, 256 };
float v_range[] = { 0, 256 };
	
	
const float* ranges[] = { h_range, s_range, v_range };
int channels[] = { 0, 1, 2 };
int hBinInit = 30, sBinInit = 32, vBinInit = 1;
int *hBin = &hBinInit, *sBin = &sBinInit, *vBin = &vBinInit;


Rect computeROI (Point p1, Point p2, Size imgSize) { 
	// when x or y coordinate exceed picture/window size, readjust them
	p2.x = p2.x<0 ? 0:p2.x; p2.x = p2.x<imgSize.width ? p2.x:imgSize.width;
	p2.y = p2.y<0 ? 0:p2.y; p2.y = p2.y<imgSize.height ? p2.y:imgSize.height;
		
	// save mouse coordinates into a range: rangeWidth(x1, x2), rangeHeight(y1,y2)
	Range rangeWidth(p1.x > p2.x ? p2.x:p1.x, p1.x > p2.x ? p1.x:p2.x);
	Range rangeHeight(p1.y > p2.y ? p2.y:p1.y, p1.y > p2.y ? p1.y:p2.y);

	cout << "rangeWidth.start = " << rangeWidth.start << "; rangeWidth.end = " << rangeWidth.end << endl;
	cout << "rangeHeight.start = " << rangeHeight.start << "; rangeHeight.end = " << rangeHeight.end << endl;

	cout << "rangeWidth.size() = " << rangeWidth.size() << endl; 
	cout << "rangeHeight.size() = " << rangeHeight.size() << endl; 
	cout << endl;

	return Rect(rangeWidth.start, rangeHeight.start, rangeWidth.size(), rangeHeight.size());
}


void printMatValues (Mat img) {
	//Print Matrix Color Values
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			cout << "(" << (int)img.at<cv::Vec3b>(i,j)[0];
			cout << "|" << (int)img.at<cv::Vec3b>(i,j)[1];
			cout << "|" << (int)img.at<cv::Vec3b>(i,j)[2] << ")" << " ";
		}
		cout << endl;
	}
}


void relativeToROIImage (Mat srcImg, vector<Mat>* histoVecOfROI, int ROISize) {
	Mat img = srcImg.clone();
	int b, g , r;
	float relativeB, relativeG, relativeR;
	
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			// Get the BGR-Values of each pixel
			b = (int) img.at<Vec3b>(i,j)[0]; 
			g = (int) img.at<Vec3b>(i,j)[1];
			r = (int) img.at<Vec3b>(i,j)[2];
			
			// calculate index for histogramm-Array
			b /= 32;
			g /= 32;
			r /= 32;

/*			// calculate Max Value in each channel
			double bMax, gMax, rMax;
			minMaxLoc((*histoVecOfROI)[0], 0, &bMax, 0, 0);
			minMaxLoc((*histoVecOfROI)[1], 0, &gMax, 0, 0);
			minMaxLoc((*histoVecOfROI)[2], 0, &rMax, 0, 0);
*/

			// calculate relative frequency for specific pixel-value
			relativeB = (*histoVecOfROI)[0].at<float>(b) / ROISize;
			relativeG = (*histoVecOfROI)[1].at<float>(g) / ROISize;
			relativeR = (*histoVecOfROI)[2].at<float>(r) / ROISize;

			// save into new image
			img.at<Vec3b>(i,j)[0] = (int) 255 * relativeB;
			img.at<Vec3b>(i,j)[1] = (int) 255 * relativeG;
			img.at<Vec3b>(i,j)[2] = (int) 255 * relativeR;
		}
	}

	// Display
 	namedWindow("Relative to ROI Image", CV_WINDOW_AUTOSIZE );
 	imshow("Relative to ROI Image", img);
	
	waitKey(0);
}

			 	 
void onMouseCallBack( int event, int x, int y, int, void* userdata) {
    	switch( event )
    	{
    	case CV_EVENT_LBUTTONDOWN:
        	origin = Point(x,y);
        	break;
    	case CV_EVENT_LBUTTONUP:
		if (userdata != NULL) {
			// calculate roi
			selection = computeROI (origin, Point (x, y), frame.size());
			roi = frame(selection);
			histogram3D();
        		selectObject = true;
			return;
		}
		// when mouse was not moved and just was clicked (i.e. no rectangle was specified), restore origin picture 
		if (origin.x == x && origin.y == y) { 
			img_untouched.copyTo(image);
			imshow ("Bild in Farbe", image);  
			destroyWindow("Histogram A4");
			destroyWindow("Histogram A5");
			return; 
		}
		// else draw a (white) rectangle in specified range
		cout << "Left button released at position x: " << x << "and y: " << y << endl;
		rectangle (image, origin, Point(x,y), Scalar(255, 255, 255)); 
		imshow ("Bild in Farbe", image);

		Rect r = computeROI(origin, Point(x,y), img_untouched.size());

		// create submatrix
		roi = img_untouched(r);

		// create histogram of submatrix
		histoVec = histogram1D(roi);

		// create relative to ROI Image
		relativeToROIImage(img_untouched, histoVec, roi.rows*roi.cols);
        	break;
    	}
}	


void trackObject () {
	TermCriteria criteria = TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 1 );

	cvtColor(frame, hsv_frame, CV_BGR2HSV); 
  	calcBackProject( &hsv_frame, 1, channels, hist, backProj, ranges, 1, true );

	imshow ("CalcBack", backProj);

	meanShift(backProj, selection, criteria);
	rectangle(frame, selection, Scalar(255,255,255));
}


void histogram3D () {	
	// define bins for hist
	int histSize[] = { *hBin, *sBin, *vBin};

	/// calculate the Histogram and normalize it
	cvtColor( roi, hsv_roi, CV_BGR2HSV );	
	calcHist( &hsv_roi, 1, channels, Mat(), hist, 3, histSize, ranges, true, false );
			
	normalize( hist, hist, 255, 0, NORM_L2, -1, Mat() );
}


void onTrackBar(int,void*) {
	if (*hBin == 0 || *sBin == 0 || *vBin == 0) { return; }
	histogram3D ();
}




















		
