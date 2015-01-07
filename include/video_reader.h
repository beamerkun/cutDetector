#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Size)
#include <string>

class VideoReader {

  public:
	virtual bool openFile(std::string& filename) = 0;
	virtual void closeFile() = 0;
	virtual std::string getCurrentFilename() = 0;
	virtual bool isOpen() = 0;

	// Returns frame with specified 0-based index.
	virtual bool getFrame(int index, cv::Mat& result) = 0;

	// Returns total number of frames in currently loaded file.
	virtual int getTotalFrameCount() = 0; 

	// Returns index of currently loaded frame inside reader.
	virtual int getCurrentFrameIndex() = 0;

	virtual int getFrameWidth() = 0;
	virtual int getFrameHeight() = 0;

}; // VideoReader
