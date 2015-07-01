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

    virtual void OnCutDetected(Frame& last, Frame& first) = 0;
    virtual void OnDifferenceCalculated(Frame& last,
                                        Frame& current,
                                        double difference) = 0;
  };  // SceneDetector::Observer

  virtual sceneList detectScenes(VideoReader* videoReader,
                                 FrameComparator* frameComparator) = 0;
  virtual void setWaitTime(int miliseconds) = 0;

  virtual void RegisterObserver(Observer* observer) = 0;
  virtual void UnregisterObserver(Observer* observer) = 0;
};  // SceneDetector
