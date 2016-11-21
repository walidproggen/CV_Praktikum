#include "myFunctions.h"
#include "histogram.h"

bool selectObject;
Rect selection;
Point origin;
Mat image, img_untouched, roi, frame;
vector<Mat>* histoVec;


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
			selection = computeROI (origin, Point (x, y), frame.size());
        		selectObject = true;
			return;
		}
		// when mouse was not moved and just was clicked (i.e. no rectangle was specified), restore origin picture 
		if (origin.x == x && origin.y == y) { 
			imshow ("Bild in Farbe", img_untouched); 
			img_untouched.copyTo(image); 
			destroyWindow("Submatrix");
			destroyWindow("histogram");
			return; 
		}
		// else draw a (white) rectangle in specified range
		cout << "Left button released at position x: " << x << "and y: " << y << endl;
		rectangle (image, origin, Point(x,y), Scalar(255, 255, 255)); 
		imshow ("Bild in Farbe", image);

		Rect r = computeROI(origin, Point(x,y), img_untouched.size());

		// create submatrix
		roi = img_untouched(r).clone();

		// create histogram of submatrix
		histoVec = histogram1D(roi);

		// create relative to ROI Image
		relativeToROIImage(img_untouched, histoVec, roi.rows*roi.cols);
        	break;
    	}
}	


void trackObjectinROI () {
	Mat hist, hsv_roi, hsv_frame, backProj, hue_roi;
	
	cvtColor( roi, hsv_roi, CV_BGR2HSV );	

  /// Use only the Hue value
  hue_roi.create( hsv_roi.size(), hsv_roi.depth() );
  int ch[] = { 0, 0 };
  mixChannels( &hsv_roi, 1, &hue_roi, 1, ch, 1 );

	int h_bins = 180; int s_bins = 32;
  	int histSize[] = { h_bins, s_bins};

  	float h_range[] = { 0, 179 };
  	float s_range[] = { 0, 255 };
  	const float* ranges[] = { h_range, s_range};

  	int channels[] = { 0, 1 };

  	/// Get the Histogram and normalize it
  	calcHist( &hsv_roi, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
	normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

	TermCriteria criteria = TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 25, 1 );

	cvtColor(frame, hsv_frame, CV_BGR2HSV); 
  	calcBackProject( &hsv_frame, 1, channels, hist, backProj, ranges, 1, true );

/*	cout << "Selection before meanshift (x, y, width, height) = ";
	cout << "(" << selection.x << ", ";
	cout << selection.y << ", ";
	cout << selection.width << ", ";
	cout << selection.y << ")" << endl; */

	meanShift(backProj, selection, criteria);
	rectangle(frame, selection, Scalar(255,255,255));

/*	cout << "Selection AFTER meanshift (x, y, width, height) = ";
	cout << "(" << selection.x << ", ";
	cout << selection.y << ", ";
	cout << selection.width << ", ";
	cout << selection.y << ")" << endl;*/

}
























		
