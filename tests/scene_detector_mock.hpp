#pragma once

#include "gmock/gmock.h"

#include "scene_detector.h"

class SceneDetectorObserverMock : public SceneDetector::Observer {
 public:
  MOCK_METHOD2(OnCutDetected,
               void(SceneDetector::Frame& last, SceneDetector::Frame& first));
  MOCK_METHOD3(OnDifferenceCalculated,
               void(SceneDetector::Frame& last,
                    SceneDetector::Frame& current,
                    double difference));
};
