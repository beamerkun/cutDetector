#include "cut_detector.hpp"

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <frame_comparator_impl.hpp>
#include <scene_detector_impl.hpp>
#include <video_reader_impl.hpp>

CutDetector::CutDetector() {
  frame_comparator_.reset(new FrameComparatorImpl());
  scene_detector_.reset(new SceneDetectorImpl());
  video_reader_.reset(new VideoReaderImpl());
}

sceneList CutDetector::detectScenes() {
  if (!video_reader_->isOpen()) {
    std::cerr << "no file opened!" << std::endl;
    return sceneList();
  }

  return scene_detector_->detectScenes(video_reader_.get(),
                                       frame_comparator_.get());
}
