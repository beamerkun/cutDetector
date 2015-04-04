#pragma once

#include <scene_detector.h>

class SceneDetectorImpl : public SceneDetector {
 public:
  // Overriden from SceneDetector
  sceneList detectScenes(VideoReader* videoReader,
                         FrameComparator* frameComparator);

  void RegisterObserver(SceneDetector::Observer* observer);
  void UnregisterObserver(SceneDetector::Observer* observer);

 private:
  std::vector<SceneDetector::Observer*> observers_;

  void OnSceneDetected(cv::Mat& lastFrame,
                       int lastFrameIndex,
                       cv::Mat& firstFrame,
                       int firstFrameIndex);
  void OnDifferenceCalculated(cv::Mat& lastFrame,
                              int lastFrameIndex,
                              cv::Mat& firstFrame,
                              int firstFrameIndex,
                              double distance);
};  // SceneDetectorImpl
