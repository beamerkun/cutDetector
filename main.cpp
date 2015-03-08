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
#include <frame_comparator_impl.hpp>
#include <video_reader_impl.hpp>

using namespace std;
using namespace cv;

bool debug = false;
const char* window1 = "ostatnia klatka sceny";
const char* window2 = "pierwsza klatka sceny";

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

	Size dimensions = Size(videoReader->getFrameWidth(),
			videoReader->getFrameHeight());

	Mat currentFrame;
	Mat lastFrame;


	std::list<std::pair<int,int> > scenes;
	if (debug) {
		int flags = CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED;
		namedWindow(window1, flags);
		namedWindow(window2);
	}
	int lastFrameIndex, currentFrameIndex;
	bool zeroBased = true;
	currentFrameIndex = 0;
	int sceneStartIndex = 0;
	int totalFrames = videoReader->getTotalFrameCount();

	videoReader->getFrame(currentFrameIndex, currentFrame);
	if(currentFrame.empty()) {
		// file indexes are not 0-based. We need to adjust.
		zeroBased = false;
		currentFrameIndex++;
		videoReader->getFrame(currentFrameIndex, currentFrame);
	}

	// TODO refactor out loop
	while(1){
		lastFrame = currentFrame.clone();	

		lastFrameIndex = currentFrameIndex;
		currentFrameIndex++;

		if (currentFrameIndex == totalFrames - 1) { 
			break;
		}
		if(!videoReader->getFrame(currentFrameIndex, currentFrame)) {
			break;
		}
		if (debug) {
			Mat lastFrameResized, currentFrameResized;
			double scale = 400.0 / dimensions.width; 
			resize(lastFrame,
					lastFrameResized,
					Size(),
					scale,
					scale,
					CV_INTER_AREA);
			resize(currentFrame,
					currentFrameResized,
					Size(),
					scale,
					scale,
					CV_INTER_AREA);
			int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
			double fontScale = 1;
			int thickness = 1;
			putText(currentFrameResized, 
					to_string(zeroBased ? currentFrameIndex : currentFrameIndex - 1),
					Point(10,30),
					fontFace, fontScale, Scalar::all(255), thickness, 8);
			putText(lastFrameResized,
					to_string(zeroBased ? lastFrameIndex : lastFrameIndex - 1),
					Point(10,30),
					fontFace, fontScale, Scalar::all(255), thickness, 8);
			imshow(window1, lastFrameResized);
			imshow(window2, currentFrameResized);
			waitKey(5000);
		}

		if(comparator->isDifferentScene(lastFrame, currentFrame, debug) ){
			int sceneEndIndex = zeroBased ? lastFrameIndex : lastFrameIndex - 1;
			std::cout << "[" << sceneStartIndex << ";" << sceneEndIndex << "]" << std::endl;
			scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));
			sceneStartIndex = sceneEndIndex + 1;
		}

	}

	int sceneEndIndex = zeroBased ? lastFrameIndex : lastFrameIndex - 1;
	std::cout << "[" << sceneStartIndex << ";" << sceneEndIndex << "]" << std::endl;
	scenes.push_back(std::make_pair(sceneStartIndex, sceneEndIndex));

	return 0;
}
