#pragma once

#include <scene_detector.h>

class SceneDetectorImpl : public SceneDetector {
 public:
  SceneDetectorImpl() : wait_time_msecs_(0) {}

  // Overriden from SceneDetector
  sceneList detectScenes(VideoReader* videoReader,
                         FrameComparator* frameComparator);
  void setWaitTime(int miliseconds);

  void RegisterObserver(SceneDetector::Observer* observer);
  void UnregisterObserver(SceneDetector::Observer* observer);

 private:
  std::vector<SceneDetector::Observer*> observers_;
  int wait_time_msecs_;

  void OnCutDetected(cv::Mat& lastFrame,
                     int lastFrameIndex,
                     cv::Mat& firstFrame,
                     int firstFrameIndex);
  void OnDifferenceCalculated(cv::Mat& lastFrame,
                              int lastFrameIndex,
                              cv::Mat& firstFrame,
                              int firstFrameIndex,
                              double distance);
};  // SceneDetectorImpl
