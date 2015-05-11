#include "cut_detector.hpp"

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <frame_comparator_impl.hpp>
#include <scene_detector_impl.hpp>
#include <video_reader_impl.hpp>

CutDetector::CutDetector() {
  command_line_.reset(new CommandLineImpl());
  frame_comparator_.reset(new FrameComparatorImpl());
  scene_detector_.reset(new SceneDetectorImpl());
  video_reader_.reset(new VideoReaderImpl());
}

sceneList CutDetector::detectScenes() {
  bool debug = command_line_->isDebug();
  std::string filename = command_line_->getFilename();
  std::string optionsFilename = command_line_->getOptionsFilename();

  if (!video_reader_->isOpen() && !video_reader_->openFile(filename)) {
    std::cerr << "failed to open file " << filename << std::endl;
    return sceneList();
  }

  if (!optionsFilename.empty())
    frame_comparator_->setOptionsFilename(optionsFilename);

  std::unique_ptr<CommandLineDebug> cl_debug;
  if (debug) {
    cl_debug.reset(new CommandLineDebug());
    scene_detector_->RegisterObserver(cl_debug.get());
  }

  return scene_detector_->detectScenes(video_reader_.get(),
                                       frame_comparator_.get());
}
