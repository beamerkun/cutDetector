#include "cut_detector_qt_interface.h"

CutDetectorQtInterface::CutDetectorQtInterface(QObject* parent)
    : QObject(parent) {
}

CutDetectorQtInterface::~CutDetectorQtInterface() {
}

void CutDetectorQtInterface::OnSceneDetected(SceneDetector::Frame& /*last*/,
                                             SceneDetector::Frame& /*first*/) {
}

void CutDetectorQtInterface::OnDifferenceCalculated(
    SceneDetector::Frame& /*last*/,
    SceneDetector::Frame& /*first*/,
    double /*difference*/) {
}
