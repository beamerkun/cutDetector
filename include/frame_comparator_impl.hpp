#include <frame_comparator.h> // filter interface

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

using namespace std;
using namespace cv;

class FrameComparatorImpl : public FrameComparator {
	public:
    struct param {
		double histogramThreshold = 0.7;
		bool limitRejects = false;
		int rejected = 5;
		int widthDiv = 16;
		int heightDiv = 9;
	};

    void setOptions(string options);
	bool isDifferentScene(Mat& lastFrame, Mat& currentFrame, bool debug);
	double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame);

	param parameters;
}; // class FrameComparatorImpl
