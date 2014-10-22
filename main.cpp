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
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

#define N 5
#define SUBSPACES 16*9
#define WIDTH_DIV 16
#define HEIGHT_DIV 9

using namespace std;
using namespace cv;

double frameDistance(Mat& last, Mat& curr);

double calculateEdgesDifference(Mat& last, Mat& curr){
	Mat sLast = last.clone();
	Mat sCurr = curr.clone();

	// KONWERSJA NA SZAROSC
	cvtColor(sLast, sLast, CV_BGR2GRAY);
	cvtColor(sCurr, sCurr, CV_BGR2GRAY);

	// WYKRYWANIE KRAWEDZI
	Sobel(sLast, sLast, -1, 1, 0);
	Sobel(sCurr, sCurr, -1, 1, 0);

	int totalEdgesPix = 0;
	int stillEdgesPix = 0;

	// szukamy pikseli krawedzi w ostatniej klatce
	// a następnie poszukujemy pikseli w obecnej klatce
	// w okolicy o rozmiarach NxN
	for( int i = 2; i < sLast.rows - 2; ++i ){
		for( int j = 2; j < sLast.cols - 2; ++j ){
			uint8_t lPix = sLast.at<uint8_t>(i ,j);
			totalEdgesPix++;
			if( lPix ){
				int ksize = (N-1)/2;
				bool stop = false;
				for( int y = i - ksize; y < i + ksize; ++y ){
					for( int x = j - ksize; x < j + ksize; ++x ){
						uint8_t cPix = sCurr.at<uint8_t>(y ,x);
						if( cPix ){
							++stillEdgesPix;
							stop = true;
							break;
						}
					}
					if( stop )
						break;
				}

			}
		}
	}

	return stillEdgesPix/(double)totalEdgesPix;
}

void calculateSubspaces(Mat& f1, Mat& f2, Mat& res){
	Size f_size = f1.size();
	int hdiv = f_size.height / HEIGHT_DIV;
	int wdiv = f_size.width / WIDTH_DIV;
	for( int h = 0; h < HEIGHT_DIV; ++h ){
		for( int w = 0; w < WIDTH_DIV; ++w ){
			Rect region = Rect( w*wdiv, h*hdiv, hdiv, wdiv );
			Mat r1 = f1(region);
			Mat r2 = f2(region);
			res.push_back( frameDistance(r1, r2) );
		}
	}
}

double frameDistance(Mat& last, Mat& curr){

	Mat channels_last[3];
	Mat channels_curr[3];

	//	frames[0] = last.clone();
	//	frames[1] = curr.clone();

	// podział klatki na 3 oddzielne kanały
	split(last, channels_last);
	split(curr, channels_curr);

	//	cvtColor(frames[0], frames[0], CV_BGR2GRAY);
	//	cvtColor(frames[1], frames[1], CV_BGR2GRAY);

	///////// OBLICZANIE HISTOGRAMU //////
	// ilosc poziomow histogrami
	int bins = 80;
	int histSize[] = {bins};
	// zakres wartosci jasnosci
	float lranges[] = { 0, 60 };
	const float* ranges[] = { lranges };
	MatND histCurr;
	MatND histLast;

	int channels[] = {0};

	double sum = 0;

	for( int i = 0; i < 3; ++i ){
		calcHist( channels_last + i, 1, channels, Mat(),
				histLast, 1, histSize, ranges,
				true, false );
		calcHist( channels_curr + i, 1, channels, Mat(),
				histCurr, 1, histSize, ranges,
				true, false );

		normalize( histCurr, histCurr, 0, 1, NORM_MINMAX, -1, Mat() );
		normalize( histLast, histLast, 0, 1, NORM_MINMAX, -1, Mat() );

		sum += compareHist( histCurr, histLast, CV_COMP_CORREL); 
	}
	
	return sum/3;
}

int main(int argc, char *argv[])
{
	VideoCapture captFile("reklama.mp4");

	if( !captFile.isOpened() ){
		std::cerr << "failed to open file" << std::endl;
		return -1;
	}

	Size dim = Size((int)captFile.get(CV_CAP_PROP_FRAME_WIDTH),
			(int)captFile.get(CV_CAP_PROP_FRAME_WIDTH));

	Mat curr;
	Mat last;
	Mat last2;

	std::list<std::pair<int,int> > scenes;

	const char* w1 = "ostatnia klatka sceny";
	const char* w2 = "pierwsza klatka sceny";

	namedWindow(w1);
	namedWindow(w2);

	double currDist;
	double currEDist;
	int lastF, currF;
	currF = 0;
	int sceneStart = 0;
	int frames = captFile.get(CV_CAP_PROP_FRAME_COUNT) - 1;
	double threshold = 20;
	double Etreshold = 0.5;

	captFile.grab();
	captFile.retrieve(curr);

	while(1){
		last = curr.clone();	

		lastF = currF;
		currF = captFile.get(CV_CAP_PROP_POS_FRAMES);
		captFile >> curr;

		if( currF == frames )
		{
			break;
		}
		Mat res;
		calculateSubspaces( last, curr, res ); 
		currEDist = calculateEdgesDifference( last, curr );

		currDist = 0;
		double m = mean(res)[0];
		for( int i = 0; i < SUBSPACES; ++i ){
			double* ptr = res.ptr<double>(i); 
			if( abs(*ptr - m) < 0.3*m )
				currDist += *ptr;
		}
		if( lastF == 0 )
			continue;
		//		std::cout << currDist << std::endl;
		//		std::cout << currEDist << std::endl;
		if( currDist < threshold && currEDist < Etreshold ){
			std::cout << "[" << sceneStart << ";" << lastF << "]" << std::endl;
			scenes.push_back(std::make_pair(sceneStart, currF));
			sceneStart = currF;
			Mat last_resize, curr_resize;
			resize(last, last_resize, Size(), 0.25, 0.25, CV_INTER_AREA);
			resize(curr, curr_resize, Size(), 0.25, 0.25, CV_INTER_AREA);
			imshow(w1, last_resize);
			imshow(w2, curr_resize);
			waitKey(10000);
		}

	}

	scenes.push_back(std::make_pair(sceneStart, currF));
	std::cout << "[" << sceneStart << ";" << currF << "]" << std::endl;

	return 0;
}
