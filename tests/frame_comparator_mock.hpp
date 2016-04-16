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
