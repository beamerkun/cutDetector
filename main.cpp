// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include <iostream>  // for standard I/O
#include <string>    // for strings
#include <memory>    // unique_ptr
#include <cstdio>    // printf

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <histogram_based_frame_comparator.hpp>
#include <adjacent_frames_scene_detector.hpp>
#include <open_cv_video_reader.hpp>
#include <cut_detector.hpp>

int main(int argc, char* argv[]) {
  std::unique_ptr<CutDetector> detector(new CutDetector());

  CommandLine* commandLine = new CommandLineImpl();

  if (!commandLine->Init(argc, argv)) {
    return -1;
  }

  detector->video_reader()->openFile(commandLine->getFilename());

  std::string optionsFilename = commandLine->getOptionsFilename();
  if (!optionsFilename.empty())
    detector->frame_comparator()->setOptionsFilename(optionsFilename);

  std::unique_ptr<CommandLineDebug> cl_debug;
  if (commandLine->isDebug()) {
    cl_debug.reset(new CommandLineDebug());
    detector->scene_detector()->RegisterObserver(cl_debug.get());
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
