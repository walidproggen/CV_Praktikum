#include "histogram.h"


void computeHistogram (Mat imgSrc) {
	// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split (imgSrc, bgr_planes);

	// number of bins
	int binSize = 8;

	// Set range for B G R
	float range[] = {0, 256};
	const float* histRange = {range};

	bool uniform = true; bool accumulate = false;

  	Mat b_hist, g_hist, r_hist;

  	/// Compute the histograms:
  	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &binSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &binSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &binSize, &histRange, uniform, accumulate );

	// create Mat for the histograms for B, G and R
  	int hist_w = 512; int hist_h = 400;
  	int bin_w = cvRound( (double) hist_w/ (binSize*3) ); // multiply by 3 because visualize 3 bars (b, g ,r) each bin, so we need maximum 24 bars

  	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(255,255,255) );

	 /// Normalize the result to [ 0, histImage.rows ]
 	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );


	// draw histogram
	// Attention: (0,0) begins at top left corner, thats why we calculate hist_h - vector[i]
	for (int i = 0; i < binSize*3; i = i += 3) {
		rectangle (histImage, Point (bin_w*i, hist_h), Point (bin_w*(i+1), hist_h - b_hist.at<float>(i/3)), Scalar(255, 0, 0), CV_FILLED);
	}

	for (int i = 1; i < binSize*3; i = i += 3) {
		rectangle (histImage, Point (bin_w*i, hist_h), Point (bin_w*(i+1), hist_h - g_hist.at<float>(i/3)), Scalar(0, 255, 0), CV_FILLED);
	}

	for (int i = 2; i < binSize*3; i = i += 3) {
		rectangle (histImage, Point (bin_w*i, hist_h), Point (bin_w*(i+1), hist_h - r_hist.at<float>(i/3)), Scalar(0, 0, 255), CV_FILLED);
	} 

 	/// Display
 	namedWindow("histogram", CV_WINDOW_AUTOSIZE );
 	imshow("histogram", histImage );

  	waitKey(0);
}