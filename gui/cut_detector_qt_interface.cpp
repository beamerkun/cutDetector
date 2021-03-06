// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include "cut_detector_qt_interface.h"

#include "cut_detector.hpp"

#include <iostream>
#include <string>

#include <QTextStream>

#include <histogram_based_frame_comparator.hpp>

CutDetectorQtInterface::CutDetectorQtInterface(QObject* parent)
    : QObject(parent), debug_(false), detector_(nullptr) {}

CutDetectorQtInterface::~CutDetectorQtInterface() {}

void CutDetectorQtInterface::OnCutDetected(SceneDetector::Frame& last,
                                           SceneDetector::Frame& first) {
  cv::Mat last_with_no, first_with_no;
  last.frame_.copyTo(last_with_no);
  first.frame_.copyTo(first_with_no);

  emit showPreviousSceneLastFrame(last_with_no);
  emit showNewSceneFirstFrame(first_with_no);
}

void CutDetectorQtInterface::OnDifferenceCalculated(
    SceneDetector::Frame& last,
    SceneDetector::Frame& current,
    double difference) {
  if (debug_) {
    std::cout << last.index_ << "->" << current.index_ << ": " << difference
              << std::endl;
  }
  emit frameDifferenceCalculated(last.index_, difference);
}

void CutDetectorQtInterface::onCurrentFrameChanged(Mat& currentFrame,
                                                   int index) {
  cv::Mat copy = currentFrame.clone();
  emit showCurrentFrame(copy);
  emit changeCurrentFrameIndex(index);
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
  if (!filename.isEmpty())
    detector_->video_reader()->openFile(filename.toStdString());
}

int CutDetectorQtInterface::getTotalFrameCount() {
  return detector_->video_reader()->getTotalFrameCount();
}

void CutDetectorQtInterface::showFrame(int index) {
  cv::Mat tmp;
  detector_->video_reader()->getFrame(index, tmp);
}

QList<QString> CutDetectorQtInterface::openCutsFile(QWidget* parent) {
  QString filename =
      QFileDialog::getOpenFileName(parent, tr("Open cuts file..."));
  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly))
    return QList<QString>();

  QTextStream stream(&file);
  QList<QString> result;

  while (true) {
    QString line = stream.readLine();

    if (line.isNull())
      break;

    result.push_back(line);
  }
  file.close();
  return result;
}

void CutDetectorQtInterface::saveCutsFile(QWidget* parent,
                                          QList<QString> cuts) {
  QString filename =
      QFileDialog::getSaveFileName(parent, tr("Choose cuts filename..."));
  QFile file(filename);

  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream stream(&file);
  for (auto line : cuts) {
    stream << line << '\n';
  }
  file.close();
}

void CutDetectorQtInterface::openSettingsJsonFile(QWidget* parent) {
  QString filename =
      QFileDialog::getOpenFileName(parent, tr("Open settings json file..."));
  if (!filename.isNull())
    detector_->frame_comparator()->setOptionsFilename(filename.toStdString());
}

void CutDetectorQtInterface::saveSettingsJsonFile(QWidget* parent) {
  QString filename =
      QFileDialog::getSaveFileName(parent, tr("Save settings json file..."));
  if (filename.isNull())
    return;
  std::string json =
      static_cast<HistogramBasedFrameComparator*>(detector_->frame_comparator())
          ->getJsonFile();

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly))
    return;

  QTextStream stream(&file);
  stream << QString::fromStdString(json);
  file.close();
}

void CutDetectorQtInterface::detectScenes() {
  emit sceneDetectionStarted();
  sceneList scenes = detector_->detectScenes();
  emit sceneListGenerated(scenes);
}

void CutDetectorQtInterface::changeWaitTime(int msecs) {
  detector_->scene_detector()->setWaitTime(msecs);
}

void CutDetectorQtInterface::rewindVideo() {
  cv::Mat temp;
  detector_->video_reader()->getFrame(0, temp);
}

void CutDetectorQtInterface::fastforwardVideo() {
  cv::Mat temp;
  detector_->video_reader()->getFrame(
      detector_->video_reader()->getTotalFrameCount() - 1, temp);
}

void CutDetectorQtInterface::stepVideoForward() {
  cv::Mat temp;
  detector_->video_reader()->getFrame(
      detector_->video_reader()->getCurrentFrameIndex() + 1, temp);
}

void CutDetectorQtInterface::stepVideoBackward() {
  cv::Mat temp;
  detector_->video_reader()->getFrame(
      detector_->video_reader()->getCurrentFrameIndex() - 1, temp);
}
