#pragma once

#include <frame_comparator.h>  // filter interface

#include <opencv2/core/core.hpp>  // cv::Mat

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

  // Overriden from FrameComparator
  void setOptionsFilename(std::string optionsFilename) override;
  bool isDifferentScene(Mat& lastFrame,
                        Mat& currentFrame,
                        double* distance) override;
  double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame) override;

  param parameters;
};  // class FrameComparatorImpl
