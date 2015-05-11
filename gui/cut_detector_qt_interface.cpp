#include "cut_detector_qt_interface.h"

#include "cut_detector.hpp"

#include <iostream>

CutDetectorQtInterface::CutDetectorQtInterface(QObject* parent)
    : QObject(parent), detector_(nullptr) {
}

CutDetectorQtInterface::~CutDetectorQtInterface() {
}

void CutDetectorQtInterface::OnCutDetected(SceneDetector::Frame& last,
                                           SceneDetector::Frame& first) {
  cv::Mat last_with_no, first_with_no;
  last.frame_.copyTo(last_with_no);
  first.frame_.copyTo(first_with_no);

  emit showNewSceneFirstFrame(first_with_no);
  emit showPreviousSceneLastFrame(last_with_no);
}

void CutDetectorQtInterface::OnDifferenceCalculated(
    SceneDetector::Frame& /*last*/,
    SceneDetector::Frame& /*first*/,
    double /*difference*/) {
}

void CutDetectorQtInterface::onCurrentFrameChanged(Mat& currentFrame,
                                                   int index) {
  emit showCurrentFrame(currentFrame.clone());
  emit changeCurrentFrameIndex(index,
                               detector_->video_reader()->getTotalFrameCount());
}

void CutDetectorQtInterface::onFileOpened(std::string /* filename */) {
  emit fileOpened();
}

void CutDetectorQtInterface::onFileClosed() {
  emit fileClosed();
}

void CutDetectorQtInterface::openVideoFile(QWidget* parent) {
  QString filename =
      QFileDialog::getOpenFileName(parent, tr("Open video file..."));
  if(!filename.isEmpty())
    detector_->video_reader()->openFile(filename.toStdString());
}

void CutDetectorQtInterface::detectScenes() {
  emit sceneDetectionStarted();
  sceneList scenes = detector_->detectScenes();
  emit sceneListGenerated(scenes);
}

void CutDetectorQtInterface::changeWaitTime(int msecs) {
  detector_->scene_detector()->setWaitTime(msecs);
}
