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
