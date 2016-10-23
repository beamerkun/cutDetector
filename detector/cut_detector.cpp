# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#include "cut_detector.hpp"

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <histogram_based_frame_comparator.hpp>
#include <adjacent_frames_scene_detector.hpp>
#include <open_cv_video_reader.hpp>

CutDetector::CutDetector() {
  frame_comparator_.reset(new HistogramBasedFrameComparator());
  scene_detector_.reset(new AdjacentFramesSceneDetector());
  video_reader_.reset(new OpenCVVideoReader());
}

sceneList CutDetector::detectScenes() {
  if (!video_reader_->isOpen()) {
    std::cerr << "no file opened!" << std::endl;
    return sceneList();
  }

  return scene_detector_->detectScenes(video_reader_.get(),
                                       frame_comparator_.get());
}
