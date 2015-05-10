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

  qRegisterMetaType<cv::Mat>("cv::Mat");
  qRegisterMetaType<QVector<int> >("QVector<int>");
  QObject::connect(&interface_, &CutDetectorQtInterface::showCurrentFrame,
                   ui->bottomOpenCVWidget, &OpenCVImageWidget::showImage);
  QObject::connect(&interface_, &CutDetectorQtInterface::showNewSceneFirstFrame,
                   ui->bottomOpenCVWidget, &OpenCVImageWidget::showImage);
  QObject::connect(&interface_,
                   &CutDetectorQtInterface::showPreviousSceneLastFrame,
                   ui->topOpenCVWidget, &OpenCVImageWidget::showImage);
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
                   [=]() { ui->sceneTableWidget->clear(); });
  QObject::connect(
      &interface_, &CutDetectorQtInterface::changeCurrentFrameIndex,
      [=](int current, int total) {
        ui->playbackSlider->setValue(((double)current / total) * 100);
      });
}

main_window::~main_window() {
  worker_thread_.exit();
  worker_thread_.wait();
  delete ui;
}
