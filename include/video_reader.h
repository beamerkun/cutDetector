// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#pragma once

#include <opencv2/core/core.hpp>  // cv::Mat

#include <string>

using cv::Mat;

class VideoReader {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    virtual void onCurrentFrameChanged(Mat& currentFrame, int index) = 0;
    virtual void onFileOpened(std::string filename) = 0;
    virtual void onFileClosed() = 0;
  };

  virtual ~VideoReader() {}

  virtual bool openFile(std::string filename) = 0;
  virtual void closeFile() = 0;
  virtual std::string getCurrentFilename() = 0;
  virtual bool isOpen() = 0;

  // Returns frame with specified 0-based index.
  virtual bool getFrame(int index, Mat& result) = 0;

  // Returns total number of frames in currently loaded file.
  virtual int getTotalFrameCount() = 0;

  // Returns index of currently loaded frame inside reader.
  virtual int getCurrentFrameIndex() = 0;

  virtual int getFrameWidth() = 0;
  virtual int getFrameHeight() = 0;

  virtual void RegisterObserver(Observer* observer) = 0;
  virtual void UnregisterObserver(Observer* observer) = 0;
};  // VideoReader
