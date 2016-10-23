# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include "gmock/gmock.h"

#include "video_reader.h"

class VideoReaderMock : public VideoReader {
 public:
  MOCK_METHOD1(openFile, bool(std::string));
  MOCK_METHOD0(closeFile, void());
  MOCK_METHOD0(getCurrentFilename, std::string());
  MOCK_METHOD0(isOpen, bool());
  MOCK_METHOD2(getFrame, bool(int index, Mat& result));
  MOCK_METHOD0(getTotalFrameCount, int());
  MOCK_METHOD0(getCurrentFrameIndex, int());
  MOCK_METHOD0(getFrameWidth, int());
  MOCK_METHOD0(getFrameHeight, int());
  MOCK_METHOD1(RegisterObserver, void(Observer* observer));
  MOCK_METHOD1(UnregisterObserver, void(Observer* observer));
};

class VideoReaderObserverMock : public VideoReader::Observer {
 public:
  MOCK_METHOD2(onCurrentFrameChanged, void(cv::Mat& frame, int index));
  MOCK_METHOD1(onFileOpened, void(std::string filename));
  MOCK_METHOD0(onFileClosed, void());
};
