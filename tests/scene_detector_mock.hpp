# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
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
