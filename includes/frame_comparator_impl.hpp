#include <frame_comparator.h> // filter interface

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

using namespace std;
using namespace cv;

class FrameComparatorImpl : public FrameComparator {
	double histogramThreshold = 20;
	double edgeTreshold = 0.5;

  public:
	bool isDifferentScene(Mat& lastFrame, Mat& currentFrame);
	double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame);

}; // class FrameComparatorImpl
