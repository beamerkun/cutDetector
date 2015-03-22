#include <scene_detector_impl.hpp>

#include <algorithm>
#include <iostream>

sceneList SceneDetectorImpl::detectScenes(VideoReader* videoReader,
                                      FrameComparator* frameComparator) {
  // TODO validate parameters

  cv::Size dimensions = cv::Size(videoReader->getFrameWidth(),
                           videoReader->getFrameHeight());
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
  if(currentFrame.empty()) {
    // file indexes are not 0-based. We need to adjust.
    offset = -1;
    currentFrameIndex++;
    videoReader->getFrame(currentFrameIndex, currentFrame);
  }

  while(1){
    lastFrame = currentFrame.clone(); 

    lastFrameIndex = currentFrameIndex;
    currentFrameIndex++;

    if (currentFrameIndex == totalFrames - 1) { 
      break;
    }
    if(!videoReader->getFrame(currentFrameIndex, currentFrame)) {
      break;
    }
    if(frameComparator->isDifferentScene(lastFrame, currentFrame, &distance) ){
      int sceneEndIndex = lastFrameIndex + offset;
      scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));
      sceneStartIndex = sceneEndIndex + 1;
      OnSceneDetected(lastFrame, sceneEndIndex, currentFrame, sceneStartIndex); 
    }
    OnDifferenceCalculated(lastFrame, lastFrameIndex + offset,
                 currentFrame, currentFrameIndex + offset,
                 distance); 
  }

  int sceneEndIndex = lastFrameIndex + offset;
  scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));

  return scenes;
}

void SceneDetectorImpl::RegisterObserver(SceneDetector::Observer* observer) {
  observers_.push_back(observer);
}

void SceneDetectorImpl::UnregisterObserver(SceneDetector::Observer* observer) {
  observers_.erase(std::remove(observers_.begin(),
                 observers_.end(),
                 observer), 
           observers_.end());
}

void SceneDetectorImpl::OnSceneDetected(cv::Mat& lastFrame, int lastFrameIndex,
                              cv::Mat& firstFrame, int firstFrameIndex) {
  SceneDetector::Frame last(lastFrame, lastFrameIndex);
  SceneDetector::Frame first(firstFrame, firstFrameIndex);
  for( auto observer : observers_ ) {
    observer->OnSceneDetected(last, first);
  }
}

void SceneDetectorImpl::OnDifferenceCalculated(cv::Mat& lastFrame, int lastFrameIndex,
                                     cv::Mat& firstFrame, int firstFrameIndex,
                         double distance) {
  SceneDetector::Frame last(lastFrame, lastFrameIndex);
  SceneDetector::Frame first(firstFrame, firstFrameIndex);
  for( auto observer : observers_ ) {
    observer->OnDifferenceCalculated(last, first, distance);
  }
}

