#include "main_window.h"
#include "ui_main_window.h"

#include <scene_detector.h>

main_window::main_window(QWidget* parent, CutDetector* detector)
    : QMainWindow(parent), ui(new Ui::main_window) {
  detector_.reset(detector);
  detector_->scene_detector()->RegisterObserver(&interface_);

  ui->setupUi(this);

  QObject::connect(&interface_, &CutDetectorQtInterface::showNewSceneFirstFrame,
                   ui->widget_2, &OpenCVImageWidget::showImage);
  QObject::connect(&interface_,
                   &CutDetectorQtInterface::showPreviousSceneLastFrame,
                   ui->widget, &OpenCVImageWidget::showImage);
  QObject::connect(ui->toolButton, &QToolButton::clicked, this,
                   &main_window::detectScenes);
  QObject::connect(this, &main_window::sceneListGenerated, [=](sceneList list) {
    for (auto scene : list) {
      ui->sceneListWidget->addItem("[" + QString::number(scene.first) + ";" +
                                   QString::number(scene.second) + "]");
    }
  });
  QObject::connect(this, &main_window::sceneDetectionStarted,
                   [=]() { ui->sceneListWidget->clear(); });
}

main_window::~main_window() {
  delete ui;
}

void main_window::detectScenes() {
  emit sceneDetectionStarted();
  sceneList scenes = detector_->detectScenes();
  emit sceneListGenerated(scenes);
}
