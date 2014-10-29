#include <frame_comparator_impl.hpp> // filter interface

#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <cstdio>
#include <list>
#include <algorithm>
#include <utility>
#include <vector>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

#define N 5
#define SUBSPACES 16*9
#define WIDTH_DIV 16
#define HEIGHT_DIV 9

using namespace std;
using namespace cv;

namespace {
	double calculateEdgesDifference(Mat& frameLHS, Mat& frameRHS){
		Mat tempLHS = frameLHS.clone();
		Mat tempRHS = frameRHS.clone();

		// Convert to greyscale.
		cvtColor(tempLHS, tempLHS, CV_BGR2GRAY);
		cvtColor(tempRHS, tempRHS, CV_BGR2GRAY);

		// Filter out edges.
		Sobel(tempLHS, tempLHS, -1, 1, 0);
		Sobel(tempRHS, tempRHS, -1, 1, 0);

		int totalEdgePixels = 0;
		int stillEdgePixels = 0;

		// Find edge pixel in left frame, then check if in right frame
		// there is still edge pixen within NxN range.
		int kSize = (N-1)/2;
		for (int i = kSize; i < tempLHS.rows - kSize; ++i ) {
			for (int j = kSize; j < tempLHS.cols - kSize; ++j) {
				uint8_t currentPixel = tempLHS.at<uint8_t>(i ,j);
				totalEdgePixels++;
				if (currentPixel) {
					bool stop = false;
					for (int y = i - kSize; y < i + kSize; ++y ) {
						for (int x = j - kSize; x < j + kSize; ++x) {
							uint8_t stillPixel = tempRHS.at<uint8_t>(y ,x);
							if (stillPixel) {
								++stillEdgePixels;
								stop = true;
								break;
							}
						}
						if (stop) {
							break;
						}
					}

				}
			}
		}

		return stillEdgePixels/(double)totalEdgePixels;
	}

}

bool FrameComparatorImpl::isDifferentScene(Mat& lastFrame, Mat& currentFrame){
	double histogramDistance, edgeDistance;

	// Divide frames into HEIGHT_DIV*WIDTH_DIV segments.
	Mat res;
	Size frameSize = lastFrame.size();
	int hDiv = frameSize.height / HEIGHT_DIV;
	int wDiv = frameSize.width / WIDTH_DIV;
	for( int h = 0; h < HEIGHT_DIV; ++h ){
		for( int w = 0; w < WIDTH_DIV; ++w ){
			Rect region = Rect( w*wDiv, h*hDiv, hDiv, wDiv );
			Mat r1 = lastFrame(region);
			Mat r2 = currentFrame(region);
			res.push_back( calculateFrameDistance(r1, r2) );
		}
	}
	edgeDistance = calculateEdgesDifference(lastFrame, currentFrame);

	histogramDistance = 0;
	double m = mean(res)[0];
	for( int i = 0; i < SUBSPACES; ++i ){
		double* ptr = res.ptr<double>(i); 
		if( abs(*ptr - m) < 0.3*m )
			histogramDistance += *ptr;
	}
#ifdef _DEBUG
	std::cout << histogramDistance << std::endl;
	std::cout << edgeDistance << std::endl;
#endif
	if (histogramDistance < histogramThreshold &&
		edgeDistance < edgeTreshold) {
		return true;
	}
	return false;
}

double FrameComparatorImpl::calculateFrameDistance(Mat& lastFrame, Mat& currentFrame){
	Mat channelsLastFrame[3];
	Mat channelsCurrentFrame[3];

	//	frames[0] = last.clone();
	//	frames[1] = curr.clone();

	// podział klatki na 3 oddzielne kanały
	split(lastFrame		, channelsLastFrame);
	split(currentFrame	, channelsCurrentFrame);

	//	cvtColor(frames[0], frames[0], CV_BGR2GRAY);
	//	cvtColor(frames[1], frames[1], CV_BGR2GRAY);

	///////// OBLICZANIE HISTOGRAMU //////
	// ilosc poziomow histogrami
	int bins = 80;
	int histSize[] = {bins};
	// zakres wartosci jasnosci
	float lranges[] = { 0, 60 };
	const float* ranges[] = { lranges };
	MatND histogramCurrent;
	MatND histogramLast;

	int channels[] = {0};

	double sum = 0;

	for( int i = 0; i < 3; ++i ){
		calcHist(channelsLastFrame + i, 1, channels, Mat(),
				histogramLast, 1, histSize, ranges,
				true, false );
		calcHist(channelsCurrentFrame + i, 1, channels, Mat(),
				histogramCurrent, 1, histSize, ranges,
				true, false );

		normalize(histogramCurrent,
				histogramCurrent,
				0, 1, NORM_MINMAX, -1, Mat() );
		normalize(histogramLast,
				histogramLast,
				0, 1, NORM_MINMAX, -1, Mat() );

		sum += compareHist(histogramCurrent,
				histogramLast,
				CV_COMP_CORREL); 
	}

	return sum/3;
}

