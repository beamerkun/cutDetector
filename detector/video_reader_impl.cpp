#include "video_reader_impl.hpp"

bool VideoReaderImpl::openFile(std::string& filename) {
  if(videoFile_.open(filename)){
    filename_ = filename;
    return true;
  }
  return false;
}

void VideoReaderImpl::closeFile() {
  videoFile_.release();
}

std::string VideoReaderImpl::getCurrentFilename() {
  return filename_;
}

bool VideoReaderImpl::isOpen() {
  return videoFile_.isOpened();
}

bool VideoReaderImpl::getFrame(int frameIndex, cv::Mat& result) {
  if(!isOpen() || frameIndex < 0 || frameIndex > getTotalFrameCount())
    return false;
  // We can't read frames backwards. Reopen file to start from beginning.
  if(frameIndex < getCurrentFrameIndex()) {
    openFile(filename_);
  }
  // Skip frames till we reach desired frameIndex.
  while(getCurrentFrameIndex() != frameIndex) {
    videoFile_.grab();
  }
  videoFile_.retrieve(result);
  return true;
}

int VideoReaderImpl::getTotalFrameCount() {
  if(isOpen())
    return videoFile_.get(CV_CAP_PROP_FRAME_COUNT);
  return 0;
}

int VideoReaderImpl::getCurrentFrameIndex() {
  if(isOpen())
    return videoFile_.get(CV_CAP_PROP_POS_FRAMES);
  return 0;
}

int VideoReaderImpl::getFrameHeight() {
  return isOpen() ? videoFile_.get(CV_CAP_PROP_FRAME_HEIGHT) : 0;
}

int VideoReaderImpl::getFrameWidth() {
  return isOpen() ? videoFile_.get(CV_CAP_PROP_FRAME_WIDTH) : 0;
}

