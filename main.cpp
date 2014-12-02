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

#include <frame_comparator_impl.hpp>

using namespace std;
using namespace cv;

bool debug = false;

int main(int argc, char *argv[]) {
	std::string filename;
	std::string options;

	try {
		TCLAP::CmdLine cmd("cutDetector", ' ', "0.1");
		TCLAP::UnlabeledValueArg<std::string> nameArg("name","filename",true,"input.mp4","string", cmd);
		TCLAP::SwitchArg debugSwitch("g","debug","enable debug mode", cmd, false);
		TCLAP::ValueArg<std::string> optionArg("o","option_string","options for frame comparator",false,"","string",cmd);
		cmd.parse(argc, argv);
		debug = debugSwitch.getValue();
		filename = nameArg.getValue();
		options = optionArg.getValue();
	} 
	catch (TCLAP::ArgException &e) { 
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
	}

	VideoCapture videoFile(filename);

	if( !videoFile.isOpened() ){
		std::cerr << "failed to open file " << filename << std::endl;
		return -1;
	}

	std::unique_ptr<FrameComparatorImpl> comparator(new FrameComparatorImpl());
	comparator->setOptions(options);

	Size dimensions = Size(
			(int)videoFile.get(CV_CAP_PROP_FRAME_WIDTH),
			(int)videoFile.get(CV_CAP_PROP_FRAME_HEIGHT));

	Mat currentFrame;
	Mat lastFrame;

	const char* window1 = "ostatnia klatka sceny";
	const char* window2 = "pierwsza klatka sceny";

	std::list<std::pair<int,int> > scenes;
	if (debug) {
		namedWindow(window1);
		namedWindow(window2);
	}
	int lastFrameIndex, currentFrameIndex;
	currentFrameIndex = 0;
	int sceneStartIndex = 0;
	int totalFrames = videoFile.get(CV_CAP_PROP_FRAME_COUNT) - 1;

	videoFile.grab();
	videoFile.retrieve(currentFrame);

	while(1){
		lastFrame = currentFrame.clone();	

		lastFrameIndex = currentFrameIndex;
		currentFrameIndex = videoFile.get(CV_CAP_PROP_POS_FRAMES);

		videoFile >> currentFrame;

		if (currentFrameIndex == totalFrames) {
			break;
		}
		if (lastFrameIndex == 0) {
			continue;
		}
		if( comparator->isDifferentScene(lastFrame, currentFrame, debug) ){
			std::cout << "[" << sceneStartIndex << ";" << lastFrameIndex << "]" << std::endl;
			scenes.push_back(std::make_pair(sceneStartIndex, currentFrameIndex));
			sceneStartIndex = currentFrameIndex;
			if (debug) {
				Mat lastFrameResized, currentFrameResized;
				resize(lastFrame,
						lastFrameResized,
						Size(),
						0.5,
						0.5,
						CV_INTER_AREA);
				resize(currentFrame,
						currentFrameResized,
						Size(),
						0.5,
						0.5,
						CV_INTER_AREA);
				imshow(window1, lastFrameResized);
				imshow(window2, currentFrameResized);
				waitKey(5000);
			}
		}

	}

	scenes.push_back(std::make_pair(sceneStartIndex, currentFrameIndex));
	std::cout << "[" << sceneStartIndex << ";" << currentFrameIndex << "]" << std::endl;

	return 0;
}
