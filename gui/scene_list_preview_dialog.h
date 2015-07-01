#ifndef SCENE_LIST_PREVIEW_DIALOG_H
#define SCENE_LIST_PREVIEW_DIALOG_H

#include <QDialog>

#include <video_reader.h>

namespace Ui {
class SceneListPreviewDialog;
}

class OpenCVImageWidget;
class QLabel;

class SceneListPreviewDialog : public QDialog {
  Q_OBJECT

 public:
  SceneListPreviewDialog(QWidget* parent = 0);
  ~SceneListPreviewDialog();

  void loadScenesList(QList<QPair<int, int>> scenes);
  QList<QString> generateScenesList();
  void setVideoReader(VideoReader* video_reader);

 private:
  void setupSignals();

  OpenCVImageWidget* getOpenCVWidgetForSceneEnd(int scene);
  OpenCVImageWidget* getOpenCVWidgetForSceneStart(int scene);

  QLabel* getFrameNumberLabelForSceneEnd(int scene);
  QLabel* getFrameNumberLabelForSceneStart(int scene);

  void updateSceneCounter();
  void updateSceneStartPreview(int scene, int frame);
  void updateSceneEndPreview(int scene, int frame);

  void clearScenePreview(int scene);

  void goToScene(int number);

  QList<QPair<int, int>> scenes_list_;

  int current_scene_;

  VideoReader* video_reader_;

  Ui::SceneListPreviewDialog* ui;
};

#endif  // SCENE_LIST_PREVIEW_DIALOG_H
