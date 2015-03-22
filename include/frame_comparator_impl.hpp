#pragma once

#include <frame_comparator.h> // filter interface

#include <algorithm>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

using cv::Mat;

class FrameComparatorImpl : public FrameComparator {
  public:
    struct param {
    double histogramThreshold = 0.7;
    bool limitRejects = false;
    int rejected = 5;
    int widthDiv = 16;
    int heightDiv = 9;
  };

  void setOptions(std::string options);
  bool isDifferentScene(Mat& lastFrame, Mat& currentFrame, double* distance);
  double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame);

  param parameters;
}; // class FrameComparatorImpl
