#include "frame_difference_graph_dialog.h"
#include "ui_frame_difference_graph_dialog.h"

FrameDifferenceGraphDialog::FrameDifferenceGraphDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::FrameDifferenceGraphDialog) {
  ui->setupUi(this);
}

FrameDifferenceGraphDialog::~FrameDifferenceGraphDialog() {
  delete ui;
}

void FrameDifferenceGraphDialog::addDifferenceValue(int frameIndex,
                                                    double difference) {
  if (frameIndex >= data_.size())
    data_.resize(frameIndex + 1);
  data_[frameIndex] = difference;
  generateGraph();
}

void FrameDifferenceGraphDialog::limitData(int limit) {
  data_.resize(limit);
  generateGraph();
}

void FrameDifferenceGraphDialog::setCuts(QList<QPair<int, int>> scenes) {
  cuts_.clear();
  for (int i = 0; i < scenes.size(); ++i) {
    cuts_.push_back(scenes[i].first);
  }
  generateGraph();
}

void FrameDifferenceGraphDialog::generateGraph() {
  QVector<double> x(data_.size());
  for (int i = 0; i < (int)data_.size(); ++i)
    x[i] = i;

  auto plotWidget = ui->plotWidget;

  plotWidget->addGraph();
  plotWidget->graph(0)->setData(x, data_);

  QVector<double> threshold_x(2),
      threshold_y(2, comparator_->getParameters().histogramThreshold);
  threshold_x[0] = 0;
  threshold_x[1] = data_.size() - 1;

  plotWidget->addGraph();
  plotWidget->graph(1)->setData(threshold_x, threshold_y);
  plotWidget->graph(1)->setPen(QPen(Qt::red));

  QVector<double> cuts_y(cuts_.size());
  for(int i = 0; i < cuts_.size(); ++i) {
    cuts_y[i] = data_[cuts_[i]];
  }
  plotWidget->addGraph();
  plotWidget->graph(2)->setData(cuts_, cuts_y);
  plotWidget->graph(2)->setPen(QPen(Qt::green));
  plotWidget->graph(2)->setLineStyle(QCPGraph::lsNone);
  plotWidget->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

  plotWidget->xAxis->setLabel("frame no");
  plotWidget->yAxis->setLabel("difference");

  plotWidget->xAxis->setRange(0, data_.size() - 1);
  plotWidget->yAxis->setRange(0, 1);

  plotWidget->replot();
}
