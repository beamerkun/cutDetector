#include "main_window.h"
#include "ui_main_window.h"

#include <QtGlobal>

#include <scene_detector.h>
#include <video_reader.h>

main_window::main_window(QWidget* parent, CutDetector* detector)
    : QMainWindow(parent),
      ui(new Ui::main_window),
      comparator_options_dialog_(new ComparatorOptionsDialog(
          this,
          static_cast<HistogramBasedFrameComparator*>(
              detector->frame_comparator()))),
      scene_list_preview_dialog_(new SceneListPreviewDialog(this)) {
  detector_.reset(detector);
  detector_->scene_detector()->RegisterObserver(&interface_);
  detector_->video_reader()->RegisterObserver(&interface_);

  scene_list_preview_dialog_->setVideoReader(detector_->video_reader());

  worker_thread_.start();
  interface_.set_detector(detector_.get());
  interface_.moveToThread(&worker_thread_);

  ui->setupUi(this);

  // Create graphs in advance
  ui->plotWidget->addGraph();
  ui->plotWidget->addGraph();
  ui->plotWidget->addGraph();

  setupSignals();
}

void main_window::setupSignals() {
  qRegisterMetaType<cv::Mat>("cv::Mat");
  qRegisterMetaType<QVector<int>>("QVector<int>");
  qRegisterMetaType<QCustomPlot::RefreshPriority>(
      "QCustomPlot::RefreshPriority");

  // Detector settings signals
  QObject::connect(ui->detectorSettingsButton, &QAbstractButton::clicked, this,
                   &main_window::openComparatorSettingsDialog);
  QObject::connect(ui->saveDetectorSettingsButton, &QAbstractButton::clicked,
                   [=]() { interface_.saveSettingsJsonFile(this); });
  QObject::connect(ui->loadDetectorSettingsButton, &QAbstractButton::clicked,
                   [=]() { interface_.openSettingsJsonFile(this); });

  // OpenCVWidget signals
  QObject::connect(&interface_, &CutDetectorQtInterface::showCurrentFrame,
                   ui->openCVWidget, &OpenCVImageWidget::showImage);

  // Scene detection signals
  QObject::connect(ui->startButton, &QToolButton::clicked, &interface_,
                   &CutDetectorQtInterface::detectScenes);
  QObject::connect(&interface_, &CutDetectorQtInterface::sceneListGenerated,
                   [=](sceneList list) {
                     for (auto scene : list) {
                       QTableWidgetItem* item1 =
                           new QTableWidgetItem(QString::number(scene.first));
                       QTableWidgetItem* item2 =
                           new QTableWidgetItem(QString::number(scene.second));
                       int row = ui->sceneTableWidget->rowCount();
                       ui->sceneTableWidget->setRowCount(
                           ui->sceneTableWidget->rowCount() + 1);
                       ui->sceneTableWidget->setItem(row, 0, item1);
                       ui->sceneTableWidget->setItem(row, 1, item2);
                     }
                   });
  QObject::connect(&interface_, &CutDetectorQtInterface::sceneDetectionStarted,
                   this, &main_window::clearScenesList);

  QObject::connect(
      &interface_, &CutDetectorQtInterface::sceneDetectionStarted, [=]() {
        graphLimitData(detector_->video_reader()->getTotalFrameCount());
      });

  // Video Reader controls
  QObject::connect(&interface_, &CutDetectorQtInterface::fileOpened, [=]() {
    ui->playbackSlider->setEnabled(true);
    ui->playbackSlider->setMinimum(0);
    ui->playbackSlider->setMaximum(this->interface_.getTotalFrameCount());
    this->graphClearData();
  });
  QObject::connect(&interface_, &CutDetectorQtInterface::fileClosed,
                   [=]() { ui->playbackSlider->setEnabled(false); });
  QObject::connect(&interface_,
                   &CutDetectorQtInterface::changeCurrentFrameIndex,
                   [=](int current, int /* total */) {
                     ui->playbackSlider->setValue(current);
                   });
  QObject::connect(ui->playbackSlider, &QSlider::sliderMoved,
                   [=](int index) { this->interface_.showFrame(index); });
  QObject::connect(ui->actionOpen_file, &QAction::triggered, [=]() {
    interface_.openVideoFile(this);
    clearScenesList();
  });
  QObject::connect(ui->stepForwardButton, &QAbstractButton::clicked,
                   &interface_, &CutDetectorQtInterface::stepVideoForward);
  QObject::connect(ui->stepBackButton, &QAbstractButton::clicked, &interface_,
                   &CutDetectorQtInterface::stepVideoBackward);
  QObject::connect(ui->rewindButton, &QAbstractButton::clicked, &interface_,
                   &CutDetectorQtInterface::rewindVideo);
  QObject::connect(ui->endButton, &QAbstractButton::clicked, &interface_,
                   &CutDetectorQtInterface::fastforwardVideo);

  // Scene list controls
  QObject::connect(ui->actionClear_cuts_list, &QAction::triggered, this,
                   &main_window::clearScenesList);
  QObject::connect(ui->clearListButton, &QAbstractButton::clicked, this,
                   &main_window::clearScenesList);
  QObject::connect(ui->loadListButton, &QAbstractButton::clicked,
                   [=]() { loadSceneList(interface_.openCutsFile(this)); });
  QObject::connect(ui->saveListButton, &QAbstractButton::clicked, [=]() {
    interface_.saveCutsFile(this, generateSceneList());
  });
  QObject::connect(ui->addRowButton, &QAbstractButton::clicked, [=]() {
    int rowCount = ui->sceneTableWidget->rowCount();
    ui->sceneTableWidget->setRowCount(++rowCount);
    ui->sceneTableWidget->setCurrentCell(rowCount - 1, 0);
  });
  QObject::connect(ui->deleteRowButton, &QAbstractButton::clicked, [=]() {
    int rowCount = ui->sceneTableWidget->rowCount();
    ui->sceneTableWidget->setRowCount(--rowCount);
    ui->sceneTableWidget->setCurrentCell(rowCount - 1, 0);
  });

  // Scene list table widget
  QObject::connect(ui->sceneTableWidget, &QTableWidget::cellChanged,
                   [=](int row, int collumn) {
                     auto item = ui->sceneTableWidget->item(row, collumn);
                     if (item->text().isEmpty())
                       return;
                     int rowCount = ui->sceneTableWidget->rowCount();
                     if (rowCount <= (row + 1)) {
                       ui->sceneTableWidget->setRowCount(++rowCount);
                     }
                     int currentVal = item->text().toInt();
                     auto newItem = new QTableWidgetItem();
                     int nextRow = collumn ? row + 1 : row;
                     int nextCol = (collumn + 1) % 2;
                     int newValue = currentVal + 1;
                     if (collumn == 0) {
                       auto nextItem =
                           ui->sceneTableWidget->item(nextRow, nextCol);
                       if (!nextItem || nextItem->text().isEmpty() ||
                           nextItem->text().toInt() > newValue) {
                         ui->sceneTableWidget->setCurrentCell(nextRow, collumn);
                         return;
                       }
                     }
                     newItem->setText(QString::number(newValue));
                     ui->sceneTableWidget->setItem(nextRow, nextCol, newItem);
                     ui->sceneTableWidget->setCurrentCell(nextRow, nextCol);
                   });

  // Scene list preview dialog
  QObject::connect(ui->openSceneListPreviewButton, &QAbstractButton::clicked,
                   [=]() {
                     QList<QString> list = generateSceneList();
                     if (!list.empty()) {
                       this->scene_list_preview_dialog_->loadScenesList(
                           sceneListStringToInt(list));
                       this->scene_list_preview_dialog_->show();
                     }
                   });

  // Frame difference graph
  QObject::connect(&interface_,
                   &CutDetectorQtInterface::frameDifferenceCalculated, this,
                   &main_window::graphAddDifferenceValue);
  QObject::connect(this, &main_window::graphNeedsUpdate, ui->plotWidget,
                   &QCustomPlot::replot);
}

QList<QString> main_window::generateSceneList() {
  ui->sceneTableWidget->blockSignals(true);
  QList<QString> result;
  for (int i = 0; i < ui->sceneTableWidget->rowCount(); ++i) {
    QString temp = "[";
    for (int j = 0; j < ui->sceneTableWidget->columnCount(); ++j) {
      if (j != 0)
        temp += ";";

      if (ui->sceneTableWidget->item(i, j))
        temp += ui->sceneTableWidget->item(i, j)->text();
      else
        temp.clear();
    }
    if (!temp.isEmpty())
      result.push_back(temp + "]");
  }
  ui->sceneTableWidget->blockSignals(false);
  return result;
}

QList<QPair<int, int>> main_window::sceneListStringToInt(
    QList<QString> scenes) {
  QList<QPair<int, int>> result;
  for (QString scene : scenes) {
    scene.remove(QChar('['));
    scene.remove(QChar(']'));
    auto cells = scene.split(";");
    result.push_back(QPair<int, int>(cells[0].toInt(), cells[1].toInt()));
  }
  return result;
}

void main_window::loadSceneList(QList<QString> list) {
  ui->sceneTableWidget->setRowCount(0);
  for (QString scene : list) {
    scene.remove(QChar('['));
    scene.remove(QChar(']'));
    auto cells = scene.split(";");
    int row = ui->sceneTableWidget->rowCount();
    ui->sceneTableWidget->setRowCount(row + 1);
    int col = 0;
    for (QString cell : cells) {
      auto item = new QTableWidgetItem(cell);
      ui->sceneTableWidget->setItem(row, col, item);
      ++col;
    }
  }
}

void main_window::clearScenesList() {
  ui->sceneTableWidget->setRowCount(0);
}

void main_window::openComparatorSettingsDialog() {
  comparator_options_dialog_->show();
}

void main_window::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Left:
      ui->stepBackButton->clicked();
      break;
    case Qt::Key_Right:
      ui->stepForwardButton->clicked();
      break;
    default:
      break;
  }
}

main_window::~main_window() {
  worker_thread_.exit();
  worker_thread_.wait();
  delete ui;
}

void main_window::graphAddDifferenceValue(int frameIndex, double difference) {
  if (frameIndex >= graph_data_.size())
    graph_data_.resize(frameIndex + 1);
  graph_data_[frameIndex] = difference;
  graphGenerateGraph();
}

void main_window::graphLimitData(int limit) {
  graph_data_.resize(limit);
  graphGenerateGraph();
}

void main_window::graphSetCuts(QList<QPair<int, int>> scenes) {
  graph_cuts_.clear();
  for (int i = 0; i < scenes.size(); ++i) {
    graph_cuts_.push_back(scenes[i].first);
  }
  graphGenerateGraph();
}

void main_window::graphGenerateGraph() {
  auto comparator = static_cast<HistogramBasedFrameComparator*>(
      detector_->frame_comparator());

  QVector<double> x(graph_data_.size());
  for (int i = 0; i < (int)graph_data_.size(); ++i)
    x[i] = i;

  auto plotWidget = ui->plotWidget;

  plotWidget->graph(0)->setData(x, graph_data_);

  QVector<double> threshold_x(2),
      threshold_y(2, comparator->getParameters().histogramThreshold);
  threshold_x[0] = 0;
  threshold_x[1] = graph_data_.size() - 1;

  plotWidget->graph(1)->setData(threshold_x, threshold_y);
  plotWidget->graph(1)->setPen(QPen(Qt::red));

  QVector<double> graph_cuts_y(graph_cuts_.size());
  for (int i = 0; i < graph_cuts_.size(); ++i) {
    graph_cuts_y[i] = graph_data_[graph_cuts_[i]];
  }
  plotWidget->graph(2)->setData(graph_cuts_, graph_cuts_y);
  plotWidget->graph(2)->setPen(QPen(Qt::green));
  plotWidget->graph(2)->setLineStyle(QCPGraph::lsNone);
  plotWidget->graph(2)
      ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

  plotWidget->xAxis->setLabel("frame no");
  plotWidget->yAxis->setLabel("similarity");

  plotWidget->xAxis->setRange(0, graph_data_.size() - 1);
  plotWidget->yAxis->setRange(0, 1);

  emit graphNeedsUpdate(QCustomPlot::RefreshPriority::rpHint);
}
