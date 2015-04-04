#include <iostream>  // for standard I/O
#include <string>    // for strings
#include <memory>    // unique_ptr
#include <cstdio>    // printf

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <frame_comparator_impl.hpp>
#include <scene_detector_impl.hpp>
#include <video_reader_impl.hpp>
#include <gui/gui.hpp>

int main(int argc, char* argv[]) {
  std::unique_ptr<CommandLine> commandLine(new CommandLineImpl());

  if (!commandLine->Init(argc, argv)) {
    return -1;
  }

  if (commandLine->isGui()) {
    gui::startGraphicsInterface(argc, argv);
    return 0;
  }

  bool debug = commandLine->isDebug();
  std::string filename = commandLine->getFilename();
  std::string optionsFilename = commandLine->getOptionsFilename();

  std::unique_ptr<VideoReader> videoReader(new VideoReaderImpl());
  if (!videoReader->openFile(filename)) {
    std::cerr << "failed to open file " << filename << std::endl;
    return -1;
  }

  std::unique_ptr<FrameComparator> comparator(new FrameComparatorImpl());
  comparator->setOptionsFilename(optionsFilename);

  std::unique_ptr<SceneDetector> detector(new SceneDetectorImpl());

  std::unique_ptr<CommandLineDebug> cl_debug;
  if (debug) {
    cl_debug.reset(new CommandLineDebug());
    detector->RegisterObserver(cl_debug.get());
  }

  sceneList scenes =
      detector->detectScenes(videoReader.get(), comparator.get());

  for (auto scene : scenes) {
    printf("[%d;%d]\n", scene.first, scene.second);
  }

  return 0;
}
