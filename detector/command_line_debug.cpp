# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#include <command_line_debug.hpp>

#include <cstdio>

void CommandLineDebug::OnCutDetected(SceneDetector::Frame& lastFrame,
                                     SceneDetector::Frame& currentFrame) {
  printf("Scene! [%d;%d]\n", lastFrame.index_, currentFrame.index_);
}

void CommandLineDebug::OnDifferenceCalculated(
    SceneDetector::Frame& lastFrame,
    SceneDetector::Frame& currentFrame,
    double difference) {
  printf("[%d;%d] difference %f\n", lastFrame.index_, currentFrame.index_,
         difference);
}
