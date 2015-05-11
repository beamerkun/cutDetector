#include "main_window.h"
#include "ui_main_window.h"

#include <scene_detector.h>

main_window::main_window(QWidget* parent, CutDetector* detector)
    : QMainWindow(parent), ui(new Ui::main_window) {
  detector_.reset(detector);
  detector_->scene_detector()->RegisterObserver(&interface_);
  detector_->video_reader()->RegisterObserver(&interface_);

  worker_thread_.start();
  interface_.set_detector(detector_.get());
  interface_.moveToThread(&worker_thread_);

  ui->setupUi(this);

  setupSignals();
}

void main_window::setupSignals() {
  qRegisterMetaType<cv::Mat>("cv::Mat");
  qRegisterMetaType<QVector<int> >("QVector<int>");

  // OpenCVWidget signals
  QObject::connect(&interface_, &CutDetectorQtInterface::showCurrentFrame,
                   ui->bottomOpenCVWidget, &OpenCVImageWidget::showImage);
  QObject::connect(&interface_, &CutDetectorQtInterface::showNewSceneFirstFrame,
                   ui->bottomOpenCVWidget, &OpenCVImageWidget::showImage);
  QObject::connect(&interface_,
                   &CutDetectorQtInterface::showPreviousSceneLastFrame,
                   ui->topOpenCVWidget, &OpenCVImageWidget::showImage);

  // Scene detection signals
  QObject::connect(ui->startButton, &QToolButton::clicked, &interface_,
                   &CutDetectorQtInterface::detectScenes);
  QObject::connect(&interface_, &CutDetectorQtInterface::sceneListGenerated,
                   [=](sceneList list) {
                     for (auto scene : list) {
                       QTableWidgetItem* item1 =
                           new QTableWidgetItem(QString::number(scene.first));
                       QTableWidgetItem* item2 =
                           new QTableWidgetItem(QString::number(scene.second));
                       int row = ui->sceneTableWidget->rowCount();
                       ui->sceneTableWidget->setRowCount(
                           ui->sceneTableWidget->rowCount() + 1);
                       ui->sceneTableWidget->setItem(row, 0, item1);
                       ui->sceneTableWidget->setItem(row, 1, item2);
                     }
                   });
  QObject::connect(&interface_, &CutDetectorQtInterface::sceneDetectionStarted,
                   this, &main_window::clearScenesList);

  // Video Reader controls
  QObject::connect(
      &interface_, &CutDetectorQtInterface::changeCurrentFrameIndex,
      [=](int current, int total) {
        ui->playbackSlider->setValue(((double)current / total) * 100);
      });
  QObject::connect(ui->actionOpen_file, &QAction::triggered,
                   [=]() { interface_.openVideoFile(this); });

  // Scene list controls
  QObject::connect(ui->actionClear_cuts_list, &QAction::triggered, this,
                   &main_window::clearScenesList);
  QObject::connect(ui->clearListButton, &QAbstractButton::clicked, this,
                   &main_window::clearScenesList);
}

void main_window::clearScenesList() {
  ui->sceneTableWidget->setRowCount(0);
}

main_window::~main_window() {
  worker_thread_.exit();
  worker_thread_.wait();
  delete ui;
}
