#include <iostream>  // for standard I/O
#include <string>    // for strings
#include <memory>    // unique_ptr
#include <cstdio>    // printf

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <frame_comparator_impl.hpp>
#include <scene_detector_impl.hpp>
#include <video_reader_impl.hpp>
#include <cut_detector.hpp>
#include <gui/gui.hpp>

int main(int argc, char* argv[]) {
  std::unique_ptr<CutDetector> detector(new CutDetector());

  CommandLine* commandLine = detector->command_line();

  if (!commandLine->Init(argc, argv)) {
    return -1;
  }

  if (commandLine->isGui()) {
    gui::startGraphicsInterface(argc, argv, detector.release());
    return 0;
  }

  sceneList scenes = detector->detectScenes();

  // Every video should contain at least one scene, something went wrong.
  if (scenes.empty())
    return -1;

  for (auto scene : scenes) {
    printf("[%d;%d]\n", scene.first, scene.second);
  }

  return 0;
}
