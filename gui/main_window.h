#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include <cut_detector.hpp>
#include <cut_detector_qt_interface.h>

namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
  Q_OBJECT

 public:
  main_window(QWidget* parent, CutDetector* detector);
  ~main_window();

  void detectScenes();

 signals:
  void sceneDetectionStarted();
  void sceneListGenerated(sceneList list);

 private:
  Ui::main_window* ui;

  std::unique_ptr<CutDetector> detector_;
  CutDetectorQtInterface interface_;
};

#endif  // MAIN_WINDOW_H
