// TODO recheck if all includes are needed
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <cstdio>
#include <list>
#include <algorithm>
#include <utility>
#include <vector>
#include <memory>   // unique_ptr

#include <tclap/CmdLine.h>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

#include <command_line_impl.hpp>
#include <command_line_debug.hpp>
#include <frame_comparator_impl.hpp>
#include <scene_detector_impl.hpp>
#include <video_reader_impl.hpp>

using namespace std;
using namespace cv;

bool debug = false;

int main(int argc, char *argv[]) {
  std::string filename;
  std::string options;

  std::unique_ptr<CommandLine> commandLine(new CommandLineImpl());

  if(!commandLine->Init(argc, argv)) {
    return -1;
  }

  debug = commandLine->isDebug();
  filename = commandLine->getFilename();
  options = commandLine->getOptionString();

  std::unique_ptr<VideoReader> videoReader(new VideoReaderImpl());
  if(!videoReader->openFile(filename)) {
    std::cerr << "failed to open file " << filename << std::endl;
    return -1;
  }

  std::unique_ptr<FrameComparator> comparator(new FrameComparatorImpl());
  comparator->setOptions(options);

  std::unique_ptr<SceneDetector> detector(new SceneDetectorImpl());

  std::unique_ptr<CommandLineDebug> cl_debug;
  if(debug) {
    cl_debug.reset(new CommandLineDebug());
    detector->RegisterObserver(cl_debug.get());
  }

  sceneList scenes = detector->detectScenes(videoReader.get(), comparator.get());

  for( auto scene : scenes ) {
    printf("[%d;%d]\n", scene.first, scene.second);
  }

  return 0;
}
