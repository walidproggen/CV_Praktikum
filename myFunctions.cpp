#include "myFunctions.h"


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
			 	 
			
