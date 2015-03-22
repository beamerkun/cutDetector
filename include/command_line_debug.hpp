#pragma once
#include <scene_detector.h>

class CommandLineDebug : public SceneDetector::Observer {
  public:
  CommandLineDebug() {};
  // Overridden from SceneDetector::Observer
  void OnSceneDetected(SceneDetector::Frame& lastFrame,
                       SceneDetector::Frame& currentFrame) override;
  void OnDifferenceCalculated(SceneDetector::Frame& lastFrame,
                              SceneDetector::Frame& currentFrame,
                              double difference) override;
};
