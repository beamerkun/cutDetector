# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include <opencv2/core/core.hpp>  // Basic OpenCV structures (cv::Mat, Scalar)

using cv::Mat;

class FrameComparator {
 public:
  virtual ~FrameComparator() {}

  virtual void setOptionsFilename(std::string optionsFilename) = 0;
  virtual double calculateFrameDistance(Mat& lastFrame, Mat& currentFrame) = 0;
  virtual bool isDifferentScene(Mat& lastFrame,
                                Mat& currentFrame,
                                double* distance) = 0;
};  // FrameComparator
