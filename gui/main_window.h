#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>

#include <comparator_options_dialog.h>
#include <cut_detector.hpp>
#include <cut_detector_qt_interface.h>
#include <scene_list_preview_dialog.h>

namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
  Q_OBJECT

 public:
  main_window(QWidget* parent, CutDetector* detector);
  ~main_window();

 public slots:
  void clearScenesList();
  void openComparatorSettingsDialog();

 private:
  void setupSignals();
  QList<QString> generateSceneList();
  void loadSceneList(QList<QString> list);

  Ui::main_window* ui;

  ComparatorOptionsDialog* comparator_options_dialog_;
  SceneListPreviewDialog* scene_list_preview_dialog_;

  QThread worker_thread_;

  std::unique_ptr<CutDetector> detector_;
  CutDetectorQtInterface interface_;
};

#endif  // MAIN_WINDOW_H
