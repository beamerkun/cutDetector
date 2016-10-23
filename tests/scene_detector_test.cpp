// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include "gmock/gmock.h"

#include <iostream>
#include <random>
#include <memory>
#ifdef WIN32
#include <windows.h> // GetTickCount
#else
#include <sys/time.h> // timeval, gettimeofday
#endif  // win32

#include "scene_detector.h"

#include "adjacent_frames_scene_detector.hpp"
#include "frame_comparator_mock.hpp"
#include "scene_detector_mock.hpp"
#include "video_reader_mock.hpp"

using testing::_;
using testing::Return;
using testing::AtLeast;

template <typename T>
class SceneDetectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    detector_.reset(new T());

    ON_CALL(comparator_mock_, isDifferentScene(_, _, _))
        .WillByDefault(Return(false));

    ON_CALL(reader_mock_, isOpen()).WillByDefault(Return(true));
    ON_CALL(reader_mock_, getFrame(_, _)).WillByDefault(Return(true));
    ON_CALL(reader_mock_, getTotalFrameCount())
        .WillByDefault(Return(frame_count_));

    detector_->RegisterObserver(&observer_mock_);
  }

  virtual void TearDown() {}

  std::unique_ptr<SceneDetector> detector_;
  SceneDetectorObserverMock observer_mock_;
  FrameComparatorMock comparator_mock_;
  VideoReaderMock reader_mock_;

  int frame_count_ = 10;
};

typedef testing::Types<AdjacentFramesSceneDetector> TestTypes;
TYPED_TEST_CASE(SceneDetectorTest, TestTypes);

TYPED_TEST(SceneDetectorTest, dontTryUsingClosedFile) {
  EXPECT_CALL(this->reader_mock_, isOpen()).WillRepeatedly(Return(false));
  EXPECT_CALL(this->reader_mock_, getFrame(_, _)).Times(0);

  sceneList result = this->detector_->detectScenes(&this->reader_mock_,
                                                   &this->comparator_mock_);
  EXPECT_EQ(result.empty(), true);
}

TYPED_TEST(SceneDetectorTest, willUseEveryFrame) {
  EXPECT_CALL(this->reader_mock_, isOpen());
  EXPECT_CALL(this->reader_mock_, getTotalFrameCount());

  EXPECT_CALL(this->comparator_mock_, isDifferentScene(_, _, _))
      .Times(AtLeast(1));

  EXPECT_CALL(this->observer_mock_, OnDifferenceCalculated(_, _, _))
      .Times(AtLeast(1));
  EXPECT_CALL(this->observer_mock_, OnCutDetected(_, _)).Times(0);

  for (int i = 0; i < this->frame_count_; i++) {
    EXPECT_CALL(this->reader_mock_, getFrame(i, _));
  }

  this->detector_->detectScenes(&this->reader_mock_, &this->comparator_mock_);
}

TYPED_TEST(SceneDetectorTest, canDetectZeroCuts) {
  EXPECT_CALL(this->reader_mock_, isOpen());
  EXPECT_CALL(this->reader_mock_, getTotalFrameCount());
  EXPECT_CALL(this->reader_mock_, getFrame(_, _))
      .Times(AtLeast(this->frame_count_));

  EXPECT_CALL(this->comparator_mock_, isDifferentScene(_, _, _))
      .Times(AtLeast(1));

  EXPECT_CALL(this->observer_mock_, OnDifferenceCalculated(_, _, _))
      .Times(AtLeast(1));
  EXPECT_CALL(this->observer_mock_, OnCutDetected(_, _)).Times(0);

  sceneList result = this->detector_->detectScenes(&this->reader_mock_,
                                                   &this->comparator_mock_);
  EXPECT_EQ(result.empty(), false);
  EXPECT_EQ(result.size(), (size_t)1);
  EXPECT_EQ(result[0], std::make_pair(0, this->frame_count_ - 1));
}

TYPED_TEST(SceneDetectorTest, canDetectOneCut) {
  EXPECT_CALL(this->reader_mock_, isOpen());
  EXPECT_CALL(this->reader_mock_, getTotalFrameCount());
  EXPECT_CALL(this->reader_mock_, getFrame(_, _))
      .Times(AtLeast(this->frame_count_));

  EXPECT_CALL(this->comparator_mock_, isDifferentScene(_, _, _))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(this->observer_mock_, OnDifferenceCalculated(_, _, _))
      .Times(AtLeast(1));
  EXPECT_CALL(this->observer_mock_, OnCutDetected(_, _)).Times(1);

  sceneList result = this->detector_->detectScenes(&this->reader_mock_,
                                                   &this->comparator_mock_);
  EXPECT_EQ(result.empty(), false);
  EXPECT_EQ(result.size(), (size_t)2);
  EXPECT_EQ(result[0].first, 0);
  EXPECT_EQ(result[1].second, this->frame_count_ - 1);
}

int getTimeMsecs() {
#ifdef WIN32
      return GetTickCount();
#else
    timeval now;
    gettimeofday(&now, 0);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif // win32
}

TYPED_TEST(SceneDetectorTest, isWaitTimeWorking) {
  EXPECT_CALL(this->reader_mock_, isOpen());
  EXPECT_CALL(this->reader_mock_, getTotalFrameCount());
  EXPECT_CALL(this->reader_mock_, getFrame(_, _))
      .Times(AtLeast(this->frame_count_));

  EXPECT_CALL(this->comparator_mock_, isDifferentScene(_, _, _))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(this->observer_mock_, OnDifferenceCalculated(_, _, _))
      .Times(AtLeast(1));
  EXPECT_CALL(this->observer_mock_, OnCutDetected(_, _)).Times(1);

  int wait_time = 50;
  this->detector_->setWaitTime(wait_time);

  int start = getTimeMsecs();
  this->detector_->detectScenes(&this->reader_mock_, &this->comparator_mock_);
  int end = getTimeMsecs();

  EXPECT_GE(end - start, wait_time);
}

TYPED_TEST(SceneDetectorTest, isRandomWaitTimeWorking) {
  EXPECT_CALL(this->reader_mock_, isOpen());
  EXPECT_CALL(this->reader_mock_, getTotalFrameCount());
  EXPECT_CALL(this->reader_mock_, getFrame(_, _))
      .Times(AtLeast(this->frame_count_));

  EXPECT_CALL(this->comparator_mock_, isDifferentScene(_, _, _))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(this->observer_mock_, OnDifferenceCalculated(_, _, _))
      .Times(AtLeast(1));
  EXPECT_CALL(this->observer_mock_, OnCutDetected(_, _)).Times(1);

  std::mt19937 rng;
  int seed = getTimeMsecs();
  rng.seed(seed);
  std::cout << "               Random seed = " << seed << std::endl;
  int wait_time = rng() % 100;
  this->detector_->setWaitTime(wait_time);

  int start = getTimeMsecs();
  this->detector_->detectScenes(&this->reader_mock_, &this->comparator_mock_);
  int end = getTimeMsecs();

  EXPECT_GE(end - start, wait_time);
}
