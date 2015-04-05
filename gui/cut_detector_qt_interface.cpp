#include "cut_detector_qt_interface.h"

CutDetectorQtInterface::CutDetectorQtInterface(QObject* parent)
    : QObject(parent) {
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
