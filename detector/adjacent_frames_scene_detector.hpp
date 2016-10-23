# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include <scene_detector.h>

class AdjacentFramesSceneDetector : public SceneDetector {
 public:
  AdjacentFramesSceneDetector() : wait_time_msecs_(0) {}

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
                              cv::Mat& currentFrame,
                              int currentFrameIndex,
                              double distance);
};  // class AdjacentFramesSceneDetector
