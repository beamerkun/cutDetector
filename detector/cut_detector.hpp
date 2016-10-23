// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#pragma once

#include <memory>

#include <command_line.h>
#include <frame_comparator.h>
#include <scene_detector.h>
#include <video_reader.h>

class CutDetector {
 public:
  CutDetector();

  VideoReader* video_reader() { return video_reader_.get(); }
  FrameComparator* frame_comparator() { return frame_comparator_.get(); }
  SceneDetector* scene_detector() { return scene_detector_.get(); }

  sceneList detectScenes();

 private:
  std::unique_ptr<SceneDetector> scene_detector_;
  std::unique_ptr<VideoReader> video_reader_;
  std::unique_ptr<FrameComparator> frame_comparator_;
};
