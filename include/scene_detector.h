#pragma once

#include <video_reader.h>
#include <frame_comparator.h>

typedef std::vector<std::pair<int, int>> sceneList;

class SceneDetector {
 public:
  virtual ~SceneDetector() {}

  struct Frame {
    Frame(Mat& frame, int index) : frame_(frame), index_(index) {}
    Mat& frame_;
    int index_;
  };  // SceneDetector::Frame

  class Observer {
   public:
    virtual ~Observer() {}

    virtual void OnSceneDetected(Frame& last, Frame& first) = 0;
    virtual void OnDifferenceCalculated(Frame& last,
                                        Frame& first,
                                        double difference) = 0;
  };  // SceneDetector::Observer

  virtual sceneList detectScenes(VideoReader* videoReader,
                                 FrameComparator* frameComparator) = 0;

  virtual void RegisterObserver(Observer* observer) = 0;
  virtual void UnregisterObserver(Observer* observer) = 0;
};  // SceneDetector
