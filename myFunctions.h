#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;


Rect computeROI (Point p1, Point p2, Size imgSize);
void printMatValues (Mat img);
void relativeToROIImage (Mat srcImg, vector<Mat>* histoVecOfROI, int ROISize);
void onMouseCallBack( int event, int x, int y, int, void* userdata);
void trackObjectinROI ();
