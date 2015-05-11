#pragma once

#include "video_reader.h"

#include <opencv/highgui.h>  // VideoCapture

class VideoReaderImpl : public VideoReader {
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
  void OnCurrentVideoFrameChanged(cv::Mat& frame, int index);

  // Currently opened file.
  cv::VideoCapture videoFile_;

  // Currently opened file filename.
  std::string filename_;

  // Observer list.
  std::vector<VideoReader::Observer*> observers_;
};  // VideoReader
