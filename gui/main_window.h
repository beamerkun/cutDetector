// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QKeyEvent>

#include <comparator_options_dialog.h>
#include <cut_detector.hpp>
#include <cut_detector_qt_interface.h>
#include <scene_list_preview_dialog.h>
#include <graph_widget_controller.h>

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

 protected:
  void keyPressEvent(QKeyEvent* event);

 private:
  void setupSignals();

  // Scene list
  QList<QString> generateSceneList();
  QList<QPair<int,int>> sceneListStringToInt(QList<QString> scenes);
  void loadSceneList(QList<QString> list);

  Ui::main_window* ui;

  ComparatorOptionsDialog* comparator_options_dialog_;
  SceneListPreviewDialog* scene_list_preview_dialog_;

  QThread worker_thread_;

  std::unique_ptr<CutDetector> detector_;
  CutDetectorQtInterface interface_;
  std::unique_ptr<GraphWidgetController> graph_controller_;
};

#endif  // MAIN_WINDOW_H
