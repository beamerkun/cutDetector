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
  int offset = 0;
  currentFrameIndex = 0;
  int sceneStartIndex = 0;
  int totalFrames = videoReader->getTotalFrameCount();
  double distance;

  videoReader->getFrame(currentFrameIndex, currentFrame);
  if (currentFrame.empty()) {
    // file indexes are not 0-based. We need to adjust.
    offset = -1;
    currentFrameIndex++;
    videoReader->getFrame(currentFrameIndex, currentFrame);
  }

  while (1) {
    lastFrame = currentFrame.clone();

    lastFrameIndex = currentFrameIndex;
    currentFrameIndex++;

    if (currentFrameIndex == totalFrames - 1) {
      break;
    }
    if (!videoReader->getFrame(currentFrameIndex, currentFrame)) {
      break;
    }
    if (frameComparator->isDifferentScene(lastFrame, currentFrame, &distance)) {
      int sceneEndIndex = lastFrameIndex + offset;
      scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));
      sceneStartIndex = sceneEndIndex + 1;
      OnCutDetected(lastFrame, sceneEndIndex, currentFrame, sceneStartIndex);
#ifdef WIN32
      Sleep(wait_time_msecs_);
#else
      usleep(wait_time_msecs_ * 1000);
#endif // win32
    }
    OnDifferenceCalculated(lastFrame, lastFrameIndex + offset, currentFrame,
                           currentFrameIndex + offset, distance);
  }

  int sceneEndIndex = lastFrameIndex + offset;
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
                                               cv::Mat& firstFrame,
                                               int firstFrameIndex,
                                               double distance) {
  SceneDetector::Frame last(lastFrame, lastFrameIndex);
  SceneDetector::Frame first(firstFrame, firstFrameIndex);
  for (auto observer : observers_) {
    observer->OnDifferenceCalculated(last, first, distance);
  }
}
