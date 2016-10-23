# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include <unordered_map>

#include "video_reader.h"

#include <opencv/highgui.h>  // VideoCapture

class OpenCVVideoReader : public VideoReader {
 public:
  // Overridden from VideoReader
  bool openFile(std::string filename) override;
  void closeFile() override;
  std::string getCurrentFilename() override;
  bool isOpen() override;
  bool getFrame(int index, cv::Mat& result) override;
  int getTotalFrameCount() override;
  int getCurrentFrameIndex() override;
  int getFrameHeight() override;
  int getFrameWidth() override;
  void RegisterObserver(Observer* observer) override;
  void UnregisterObserver(Observer* observer) override;

 private:
  class FrameCache {
   public:
    void storeFrame(int index, cv::Mat& frame);
    bool getFrame(int index, cv::Mat& result);

   private:
    const size_t frameCacheSize = 100;
    std::map<int, cv::Mat> cache_;
  };

  void OnCurrentVideoFrameChanged(cv::Mat& frame, int index);
  void OnFileOpened(std::string& filename);
  void OnFileClosed();

  // Currently opened file.
  cv::VideoCapture videoFile_;

  // Currently opened file filename.
  std::string filename_;

  // Frame offset.
  int offset_;

  // Last returned frame.
  int last_returned_frame_;

  FrameCache cache_;

  // Observer list.
  std::vector<VideoReader::Observer*> observers_;
};  // OpenCVVideoReader
