#ifndef SCENE_LIST_PREVIEW_DIALOG_H
#define SCENE_LIST_PREVIEW_DIALOG_H

#include <QDialog>

#include <video_reader.h>

namespace Ui {
class SceneListPreviewDialog;
}

class SceneListPreviewDialog : public QDialog {
  Q_OBJECT

 public:
  SceneListPreviewDialog(QWidget* parent = 0);
  ~SceneListPreviewDialog();

  void loadScenesList(QList<QString> scenes);
  QList<QString> generateScenesList();
  void setVideoReader(VideoReader* video_reader);

 private:
  void setupSignals();

  void updateSceneCounter();
  void updateSceneStartPreview(int frame);
  void updateSceneEndPreview(int frame);

  void goToScene(int number);

  QList<QPair<int, int>> scenes_list_;

  int current_scene_;

  VideoReader* video_reader_;

  Ui::SceneListPreviewDialog* ui;
};

#endif  // SCENE_LIST_PREVIEW_DIALOG_H
