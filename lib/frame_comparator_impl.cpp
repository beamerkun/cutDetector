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

using namespace std;
using namespace cv;

namespace {
	int N = 5;
	int WIDTH_DIV = 16;
	int HEIGHT_DIV = 9;
	int SUBSPACES = WIDTH_DIV * HEIGHT_DIV;
	void colorSubspaces(Mat& frame1, Mat& frame2, Mat& result) {
		Size frameSize = frame1.size();
		int hDiv = frameSize.height / HEIGHT_DIV;
		int wDiv = frameSize.width / WIDTH_DIV;
		double m = mean(result)[0];
		for( int h = 0; h < HEIGHT_DIV; ++h ){
			for( int w = 0; w < WIDTH_DIV; ++w ){
				double* ptr = result.ptr<double>(h*WIDTH_DIV+w); 
				if( abs(*ptr - m) < 0.3*m && false) {
					Rect region = Rect( w*wDiv, h*hDiv, wDiv, hDiv );
					rectangle(frame1, region, Scalar(0,0,255,0.3), CV_FILLED);
				}
			}
		}
	}
}

void FrameComparatorImpl::setOptions(string options) {
	stringstream stringStream(options);
	if(stringStream.good())
		stringStream >> histogramThreshold;
}

bool FrameComparatorImpl::isDifferentScene(Mat& lastFrame, Mat& currentFrame, bool debug){
	double histogramDistance;

	// Divide frames into HEIGHT_DIV*WIDTH_DIV segments.
	Mat result;
	Size frameSize = lastFrame.size();
	int hDiv = frameSize.height / HEIGHT_DIV;
	int wDiv = frameSize.width / WIDTH_DIV;
	for( int h = 0; h < HEIGHT_DIV; ++h ){
		for( int w = 0; w < WIDTH_DIV; ++w ){
			Rect region = Rect( w*wDiv, h*hDiv, wDiv, hDiv );
			Mat r1 = lastFrame(region);
			Mat r2 = currentFrame(region);
			result.push_back( calculateFrameDistance(r1, r2) );
		}
	}

	histogramDistance = 0;
	int count = 0;
	double m = mean(result)[0];
	for( int i = 0; i < SUBSPACES; ++i ){
		double* ptr = result.ptr<double>(i); 
		if( abs(*ptr - m) < 0.3*m ){
			histogramDistance += *ptr;
			count++;
		}
	}
	if(debug && false)
		std::cout << histogramDistance/count << " t: " << histogramThreshold << std::endl;
	if (histogramDistance/count < histogramThreshold) {
		if(debug)
			colorSubspaces(lastFrame, currentFrame, result);
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

