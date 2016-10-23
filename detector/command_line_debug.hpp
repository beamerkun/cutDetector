# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include <scene_detector.h>

class CommandLineDebug : public SceneDetector::Observer {
 public:
  CommandLineDebug() {}
  // Overridden from SceneDetector::Observer
  void OnCutDetected(SceneDetector::Frame& lastFrame,
                     SceneDetector::Frame& currentFrame) override;
  void OnDifferenceCalculated(SceneDetector::Frame& lastFrame,
                              SceneDetector::Frame& currentFrame,
                              double difference) override;
};
