#include "histogram.h"
	

vector<Mat>* computeHistogram (Mat imgSrc) {	
	// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split (imgSrc, bgr_planes);
	
	//Image Size
	float imgSize = imgSrc.rows * imgSrc.cols;

	// number of bins
	int binSize = 8;

	// Set range for B G R
	float range[] = {0, 256};
	const float* histRange = {range};

	bool uniform = true; bool accumulate = false;

  	Mat b_hist, g_hist, r_hist;
	Mat b_hist_org, g_hist_org, r_hist_org; 

  	/// Compute the histograms:
  	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &binSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &binSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &binSize, &histRange, uniform, accumulate );

	//Save the histograms and return at end of function
	vector<Mat>* histogramVec = new vector<Mat>(3);
	(*histogramVec)[0] = b_hist.clone();
	(*histogramVec)[1] = g_hist.clone();	
	(*histogramVec)[2] = r_hist.clone();	

	b_hist_org = b_hist.clone(); g_hist_org = g_hist.clone(); r_hist_org = r_hist.clone();

	// create Mat for the histogram
  	int hist_w = 512; int hist_h = 400;
  	int bin_w = cvRound( (double) hist_w/ (binSize*3) ); // multiply by 3 because visualize 3 bars (b, g ,r) each bin, so we need maximum 24 bars
	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(255,255,255) );

	//Normalize from 0 - 100% (hist_h = 100%)
	for (int i = 0; i < binSize; i++) {
		b_hist.at<float>(i) = b_hist.at<float>(i) == 0 ? 0:(400.0f / (imgSize/b_hist.at<float>(i)));
		g_hist.at<float>(i) = g_hist.at<float>(i) == 0 ? 0:(400.0f / (imgSize/g_hist.at<float>(i)));
		r_hist.at<float>(i) = r_hist.at<float>(i) == 0 ? 0:(400.0f / (imgSize/r_hist.at<float>(i)));
	}

	 /// Normalize the result to [ 0, histImage.rows ]
/* 	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
*/

	// draw histogram
	// Attention: (0,0) begins at top left corner, thats why we calculate hist_h - vector[i]
	for (int i = 0; i < binSize*3; i = i += 3) {
		rectangle (histImage, Point (bin_w*i, hist_h), Point (bin_w*(i+1), hist_h - b_hist.at<float>(i/3)), Scalar(255, 0, 0), CV_FILLED);
		cout << "Blau-Kanal  ["<< i/3 << "] = " << (int) b_hist.at<float>(i/3) << "		vs " << b_hist_org.at<float>(i/3) << endl;

		rectangle (histImage, Point (bin_w*(i+1), hist_h), Point (bin_w*(i+1+1), hist_h - g_hist.at<float>(i/3)), Scalar(0, 255, 0), CV_FILLED);
		cout << "Gruen-Kanal ["<< i/3 << "] = " << (int) g_hist.at<float>(i/3) << "		vs " << g_hist_org.at<float>(i/3) << endl;

		rectangle (histImage, Point (bin_w*(i+2), hist_h), Point (bin_w*(i+2+1), hist_h - r_hist.at<float>(i/3)), Scalar(0, 0, 255), CV_FILLED);
		cout << "rot-Kanal   ["<< i/3 << "] = " << (int) r_hist.at<float>(i/3) << "		vs " << r_hist_org.at<float>(i/3) << endl;

		cout << endl;
	}

 	// Display
 	namedWindow("histogram", CV_WINDOW_AUTOSIZE );
 	imshow("histogram", histImage );

	return histogramVec;
}
