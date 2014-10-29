#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)

using cv::Mat;

class FrameComparator {
  public:
	virtual double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame) = 0;
	virtual bool isDifferentScene(Mat& lastFrame, Mat& currentFrame) = 0;
}; // FrameComparator
