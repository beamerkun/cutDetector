// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include "gmock/gmock.h"

#include "scene_detector.h"

#include "adjacent_frames_scene_detector.hpp"
#include "video_reader_mock.hpp"

using testing::_;
using testing::Return;
using testing::AtLeast;

class HistogramBasedFrameComparatorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}

  virtual void TearDown() {}

  VideoReaderMock reader_mock_;
};
