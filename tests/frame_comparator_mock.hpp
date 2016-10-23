# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include "gmock/gmock.h"

#include "frame_comparator.h"

class FrameComparatorMock : public FrameComparator {
 public:
  MOCK_METHOD1(setOptionsFilename, void(std::string file));
  MOCK_METHOD2(calculateFrameDistance,
               double(Mat& lastFrame, Mat& currentFrame));
  MOCK_METHOD3(isDifferentScene,
               bool(Mat& lastFrame, Mat& currentFrame, double* distance));
};
