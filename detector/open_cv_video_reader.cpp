#include <open_cv_video_reader.hpp>

bool OpenCVVideoReader::openFile(std::string filename) {
  if (videoFile_.open(filename)) {
    filename_ = filename;
    cv::Mat temp;
    videoFile_.retrieve(temp);
    offset_ = temp.empty() ? -1 : 0;
    return true;
  }
  return false;
}

void OpenCVVideoReader::closeFile() {
  videoFile_.release();
}

std::string OpenCVVideoReader::getCurrentFilename() {
  return filename_;
}

bool OpenCVVideoReader::isOpen() {
  return videoFile_.isOpened();
}

bool OpenCVVideoReader::getFrame(int frameIndex, cv::Mat& result) {
  if (!isOpen() || frameIndex < 0 || frameIndex > getTotalFrameCount())
    return false;
  if (frameIndex > getCurrentFrameIndex()) {
    // Manually grabbing frames forward is much more faster than
    // setting CV_CAP_PROP_POS_FRAMES
    while (getCurrentFrameIndex() != frameIndex)
      videoFile_.grab();
  } else if (frameIndex < getCurrentFrameIndex()) {
    videoFile_.set(CV_CAP_PROP_POS_FRAMES, frameIndex - offset_);
  }
  videoFile_.retrieve(result);
  OnCurrentVideoFrameChanged(result, frameIndex);
  return true;
}

int OpenCVVideoReader::getTotalFrameCount() {
  if (isOpen())
    return (static_cast<int>(videoFile_.get(CV_CAP_PROP_FRAME_COUNT)) +
            offset_);
  return 0;
}

int OpenCVVideoReader::getCurrentFrameIndex() {
  if (isOpen())
    return (static_cast<int>(videoFile_.get(CV_CAP_PROP_POS_FRAMES)) + offset_);
  return 0;
}

int OpenCVVideoReader::getFrameHeight() {
  return isOpen() ? static_cast<int>(videoFile_.get(CV_CAP_PROP_FRAME_HEIGHT))
                  : 0;
}

int OpenCVVideoReader::getFrameWidth() {
  return isOpen() ? static_cast<int>(videoFile_.get(CV_CAP_PROP_FRAME_WIDTH))
                  : 0;
}

void OpenCVVideoReader::OnCurrentVideoFrameChanged(Mat& frame, int index) {
  for (auto observer : observers_) {
    observer->onCurrentFrameChanged(frame, index);
  }
}

void OpenCVVideoReader::RegisterObserver(VideoReader::Observer* observer) {
  observers_.push_back(observer);
}

void OpenCVVideoReader::UnregisterObserver(VideoReader::Observer* observer) {
  observers_.erase(std::remove(observers_.begin(), observers_.end(), observer),
                   observers_.end());
}
