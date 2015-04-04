#pragma once

#include <opencv2/core/core.hpp>  // Basic OpenCV structures (cv::Mat, Scalar)

using cv::Mat;

class FrameComparator {
 public:
  virtual void setOptionsFilename(std::string optionsFilename) = 0;
  virtual double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame) = 0;
  virtual bool isDifferentScene(Mat& lastFrame,
                                Mat& currentFrame,
                                double* distance) = 0;
};  // FrameComparator
