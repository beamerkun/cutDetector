// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include <adjacent_frames_scene_detector.hpp>

#include <algorithm>  // std::make_pair
#ifdef WIN32
#include <windows.h> // sleep
#else
#include <unistd.h> // usleep
#endif // win32

sceneList AdjacentFramesSceneDetector::detectScenes(VideoReader* videoReader,
                                          FrameComparator* frameComparator) {
  if (!videoReader->isOpen()) {
    return sceneList();
  }

  cv::Mat currentFrame;
  cv::Mat lastFrame;
  sceneList scenes;
  int lastFrameIndex, currentFrameIndex;
  currentFrameIndex = 0;
  int sceneStartIndex = 0;
  int totalFrames = videoReader->getTotalFrameCount();
  double distance;

  videoReader->getFrame(currentFrameIndex, currentFrame);

  while (1) {
    lastFrame = currentFrame.clone();

    lastFrameIndex = currentFrameIndex;
    currentFrameIndex++;

    if (lastFrameIndex == totalFrames - 1) {
      break;
    }
    if (!videoReader->getFrame(currentFrameIndex, currentFrame)) {
      break;
    }
    if (frameComparator->isDifferentScene(lastFrame, currentFrame, &distance)) {
      int sceneEndIndex = lastFrameIndex;
      scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));
      sceneStartIndex = sceneEndIndex + 1;
      OnCutDetected(lastFrame, sceneEndIndex, currentFrame, sceneStartIndex);
#ifdef WIN32
      Sleep(wait_time_msecs_);
#else
      usleep(wait_time_msecs_ * 1000);
#endif // win32
    }
    OnDifferenceCalculated(lastFrame, lastFrameIndex, currentFrame,
                           currentFrameIndex, distance);
  }

  int sceneEndIndex = lastFrameIndex;
  scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));

  return scenes;
}

void AdjacentFramesSceneDetector::setWaitTime(int miliseconds) {
  wait_time_msecs_ = miliseconds;
}

void AdjacentFramesSceneDetector::RegisterObserver(SceneDetector::Observer* observer) {
  observers_.push_back(observer);
}

void AdjacentFramesSceneDetector::UnregisterObserver(SceneDetector::Observer* observer) {
  observers_.erase(std::remove(observers_.begin(), observers_.end(), observer),
                   observers_.end());
}

void AdjacentFramesSceneDetector::OnCutDetected(cv::Mat& lastFrame,
                                      int lastFrameIndex,
                                      cv::Mat& firstFrame,
                                      int firstFrameIndex) {
  SceneDetector::Frame last(lastFrame, lastFrameIndex);
  SceneDetector::Frame first(firstFrame, firstFrameIndex);
  for (auto observer : observers_) {
    observer->OnCutDetected(last, first);
  }
}

void AdjacentFramesSceneDetector::OnDifferenceCalculated(cv::Mat& lastFrame,
                                               int lastFrameIndex,
                                               cv::Mat& currentFrame,
                                               int currentFrameIndex,
                                               double distance) {
  SceneDetector::Frame last(lastFrame, lastFrameIndex);
  SceneDetector::Frame current(currentFrame, currentFrameIndex);
  for (auto observer : observers_) {
    observer->OnDifferenceCalculated(last, current, distance);
  }
}
