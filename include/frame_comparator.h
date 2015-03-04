#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)

using cv::Mat;

class FrameComparator {
  public:
	// TODO think out better way to pass options
	virtual void setOptions(std::string options) {}
	virtual double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame) = 0;
	virtual bool isDifferentScene(Mat& lastFrame, Mat& currentFrame, bool debug) = 0;
}; // FrameComparator
