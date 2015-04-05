#include "main_window.h"
#include "ui_main_window.h"

#include <scene_detector.h>

main_window::main_window(QWidget* parent, CutDetector *detector)
    : QMainWindow(parent), ui(new Ui::main_window) {
  detector_.reset(detector);
  detector_->scene_detector()->RegisterObserver(&interface_);

  ui->setupUi(this);
}

main_window::~main_window() {
  delete ui;
}
