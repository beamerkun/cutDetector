#include "scene_list_preview_dialog.h"
#include "ui_scene_list_preview_dialog.h"

SceneListPreviewDialog::SceneListPreviewDialog(QWidget* parent)
    : QDialog(parent),
      video_reader_(nullptr),
      ui(new Ui::SceneListPreviewDialog) {
  ui->setupUi(this);

  setupSignals();
}

void SceneListPreviewDialog::setupSignals() {
  // Scene controls
  QObject::connect(ui->currentSceneControlsStepForwardButton,
                   &QAbstractButton::clicked,
                   [=]() { goToScene(current_scene_ + 1); });

  QObject::connect(ui->currentSceneControlsStepBackButton,
                   &QAbstractButton::clicked,
                   [=]() { goToScene(current_scene_ - 1); });

  QObject::connect(ui->currentSceneControlsGoStartButton,
                   &QAbstractButton::clicked, [=]() { goToScene(0); });

  QObject::connect(ui->currentSceneControlsGoEndButton,
                   &QAbstractButton::clicked,
                   [=]() { goToScene(scenes_list_.size() - 1); });
}

OpenCVImageWidget* SceneListPreviewDialog::getOpenCVWidgetForSceneEnd(
    int scene) {
  return (scene == 1 ? ui->secondSceneEndPreviewOpenCVImageWidget
                     : ui->firstSceneEndPreviewOpenCVImageWidget);
}

QLabel* SceneListPreviewDialog::getFrameNumberLabelForSceneEnd(int scene) {
  return (scene == 1 ? ui->secondSceneEndPreviewCurrentFrameNumberLabel
                     : ui->firstSceneEndPreviewCurrentFrameNumberLabel);
}

OpenCVImageWidget* SceneListPreviewDialog::getOpenCVWidgetForSceneStart(
    int scene) {
  return (scene == 1 ? ui->secondSceneStartPreviewOpenCVImageWidget
                     : ui->firstSceneStartPreviewOpenCVImageWidget);
}

QLabel* SceneListPreviewDialog::getFrameNumberLabelForSceneStart(int scene) {
  return (scene == 1 ? ui->secondSceneStartPreviewCurrentFrameNumberLabel
                     : ui->firstSceneStartPreviewCurrentFrameNumberLabel);
}

void SceneListPreviewDialog::setVideoReader(VideoReader* video_reader) {
  video_reader_ = video_reader;
}

void SceneListPreviewDialog::updateSceneCounter() {
  ui->currentSceneControlsLabel->setText(
      QString::number(current_scene_) + " / " +
      QString::number(scenes_list_.size() - 1));
}

void SceneListPreviewDialog::updateSceneStartPreview(int scene, int frame) {
  cv::Mat _tmp;
  video_reader_->getFrame(frame, _tmp);
  getOpenCVWidgetForSceneStart(scene)->showImage(_tmp);

  getFrameNumberLabelForSceneStart(scene)->setText(QString::number(frame));
}

void SceneListPreviewDialog::updateSceneEndPreview(int scene, int frame) {
  cv::Mat _tmp;
  video_reader_->getFrame(frame, _tmp);
  getOpenCVWidgetForSceneEnd(scene)->showImage(_tmp);

  getFrameNumberLabelForSceneEnd(scene)->setText(QString::number(frame));
}

void SceneListPreviewDialog::loadScenesList(QList<QString> scenes) {
  scenes_list_.clear();
  for (QString scene : scenes) {
    scene.remove(QChar('['));
    scene.remove(QChar(']'));
    auto cells = scene.split(";");
    scenes_list_.push_back(QPair<int, int>(cells[0].toInt(), cells[1].toInt()));
  }
  current_scene_ = 0;

  goToScene(0);
}

QList<QString> SceneListPreviewDialog::generateScenesList() {
  QList<QString> result;
  for (QPair<int, int> pair : scenes_list_) {
    result.push_back("[" + QString::number(pair.first) + ";" +
                     QString::number(pair.second) + "]\n");
  }
  return result;
}

void SceneListPreviewDialog::clearScenePreview(int scene) {
  getOpenCVWidgetForSceneEnd(scene)->showImage(cv::Mat());
  getOpenCVWidgetForSceneStart(scene)->showImage(cv::Mat());
  getFrameNumberLabelForSceneEnd(scene)->setText("0");
  getFrameNumberLabelForSceneStart(scene)->setText("0");
}

void SceneListPreviewDialog::goToScene(int number) {
  if (number >= scenes_list_.size() || number < 0)
    return;
  current_scene_ = number;

  updateSceneCounter();
  updateSceneStartPreview(0, scenes_list_[current_scene_].first);
  updateSceneEndPreview(0, scenes_list_[current_scene_].second);

  int next_scene = current_scene_ + 1;
  if (next_scene < scenes_list_.size()) {
    updateSceneStartPreview(1, scenes_list_[next_scene].first);
    updateSceneEndPreview(1, scenes_list_[next_scene].second);
  } else {
    clearScenePreview(1);
  }
}

SceneListPreviewDialog::~SceneListPreviewDialog() {
  delete ui;
}
