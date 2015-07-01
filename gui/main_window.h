#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QKeyEvent>

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

  void graphClearData() { graph_data_.clear(); }
  void graphLimitData(int limit);
  void graphSetCuts(QList<QPair<int,int>> scenes);

 public slots:
  void graphAddDifferenceValue(int frameIndex, double difference);

 protected:
  void keyPressEvent(QKeyEvent* event);

 private:
  void setupSignals();

  // Scene list
  QList<QString> generateSceneList();
  QList<QPair<int,int>> sceneListStringToInt(QList<QString> scenes);
  void loadSceneList(QList<QString> list);

  // Graph
  void graphGenerateGraph();

  Ui::main_window* ui;

  ComparatorOptionsDialog* comparator_options_dialog_;
  SceneListPreviewDialog* scene_list_preview_dialog_;

  QThread worker_thread_;

  QVector<double> graph_data_;
  QVector<double> graph_cuts_;

  std::unique_ptr<CutDetector> detector_;
  CutDetectorQtInterface interface_;
};

#endif  // MAIN_WINDOW_H
