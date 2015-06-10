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

void SceneListPreviewDialog::setVideoReader(VideoReader* video_reader) {
  video_reader_ = video_reader;
}

void SceneListPreviewDialog::updateSceneCounter() {
  ui->currentSceneControlsLabel->setText(
      QString::number(current_scene_) + " / " +
      QString::number(scenes_list_.size() - 1));
}

void SceneListPreviewDialog::updateSceneStartPreview(int frame) {
  cv::Mat _tmp;
  video_reader_->getFrame(frame, _tmp);
  ui->sceneStartPreviewOpenCVImageWidget->showImage(_tmp);

  ui->sceneStartPreviewCurrentFrameNumberLabel->setText(QString::number(frame));
}

void SceneListPreviewDialog::updateSceneEndPreview(int frame) {
  cv::Mat _tmp;
  video_reader_->getFrame(frame, _tmp);
  ui->sceneEndPreviewOpenCVImageWidget->showImage(_tmp);

  ui->sceneEndPreviewCurrentFrameNumberLabel->setText(QString::number(frame));
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

void SceneListPreviewDialog::goToScene(int number) {
  if (number >= scenes_list_.size() || number < 0)
    return;
  current_scene_ = number;

  updateSceneCounter();
  updateSceneStartPreview(scenes_list_[current_scene_].first);
  updateSceneEndPreview(scenes_list_[current_scene_].second);
}

SceneListPreviewDialog::~SceneListPreviewDialog() {
  delete ui;
}
