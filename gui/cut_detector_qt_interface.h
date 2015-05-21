#pragma once

#include <QObject>
#include <QFileDialog>

#include <adjacent_frames_scene_detector.hpp>

class CutDetector;

class CutDetectorQtInterface : public QObject,
                               public SceneDetector::Observer,
                               public VideoReader::Observer {
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

  // Overriden from VideoReader::Observer
  void onCurrentFrameChanged(Mat& currentFrame, int index) override;
  void onFileOpened(std::string filename) override;
  void onFileClosed() override;

  void openVideoFile(QWidget* parent);

  QList<QString> openCutsFile(QWidget* parent);
  void saveCutsFile(QWidget* parent, QList<QString> cuts);

  void openSettingsJsonFile(QWidget* parent);
  void saveSettingsJsonFile(QWidget* parent);

  void set_detector(CutDetector* detector) { detector_ = detector; }

 signals:
  void fileOpened();
  void fileClosed();
  void changeCurrentFrameIndex(int index, int frameCount);
  void sceneDetectionStarted();
  void sceneListGenerated(sceneList list);
  void showCurrentFrame(const cv::Mat frame);
  void showNewSceneFirstFrame(const cv::Mat frame);
  void showPreviousSceneLastFrame(const cv::Mat frame);

 public slots:
  void detectScenes();
  void changeWaitTime(int msecs);
  void rewindVideo();
  void fastforwardVideo();
  void stepVideoForward();
  void stepVideoBackward();

 private:
  void putFrameIndex(cv::Mat& frame, int index);

  CutDetector* detector_;
};
