#pragma once

#include <QObject>

#include <scene_detector_impl.hpp>

class CutDetectorQtInterface : public QObject, public SceneDetector::Observer {
  Q_OBJECT
 public:
  explicit CutDetectorQtInterface(QObject* parent = 0);
  ~CutDetectorQtInterface();

  // Overriden from SceneDetector::Observer
  void OnCutDetected(SceneDetector::Frame& last,
                     SceneDetector::Frame& first) override;
  void OnDifferenceCalculated(SceneDetector::Frame& last,
                              SceneDetector::Frame& first,
                              double difference) override;

 signals:
  void showNewSceneFirstFrame(cv::Mat& frame);
  void showPreviousSceneLastFrame(cv::Mat& frame);

 public slots:
};
