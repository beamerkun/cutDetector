#include "graph_widget_controller.h"

#include <histogram_based_frame_comparator.hpp>

namespace {
double cCurrentIndexLabelYPosition = 1.01;
int cCurrentIndexLabelHeight = 20;
int cGraphRightMargin = 30;
}

GraphWidgetController::GraphWidgetController(QCustomPlot* widget,
                                             CutDetector* detector)
    : widget_(widget), detector_(detector) {}

void GraphWidgetController::graphAddDifferenceValue(int frameIndex,
                                                    double difference) {
  if (frameIndex >= graph_data_.size())
    graph_data_.resize(frameIndex + 1);
  graph_data_[frameIndex] = difference;
  graphGenerateGraph();
}

void GraphWidgetController::graphLimitData(int limit) {
  graph_data_.resize(limit);
  graphGenerateGraph();
}

void GraphWidgetController::graphSetCuts(QList<QPair<int, int>> scenes) {
  graph_cuts_.clear();
  for (int i = 0; i < scenes.size(); ++i) {
    graph_cuts_.push_back(scenes[i].first);
  }
  graphGenerateGraph();
}

void GraphWidgetController::graphSetCurrentFrame(int index) {
  double d_index = static_cast<double>(index);
  widget_->graph(GraphLayer::CURRENT_FRAME)
      ->setData(QVector<double>({d_index - 1.0, d_index, d_index + 1.0}),
                QVector<double>({0.0, 1.0, 0.0}));
  widget_->graph(GraphLayer::CURRENT_FRAME)->setPen(QPen(Qt::green));

  auto indexText = static_cast<QCPItemText*>(widget_->item(0));
  indexText->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
  indexText->position->setType(QCPItemPosition::ptPlotCoords);
  indexText->position->setCoords(d_index, cCurrentIndexLabelYPosition);
  indexText->setText(QString::number(index));
  indexText->setPen(QPen(Qt::black));
  indexText->setBrush(QBrush(Qt::white));
  indexText->setClipToAxisRect(false);

  emit graphNeedsUpdate(QCustomPlot::RefreshPriority::rpHint);
}

void GraphWidgetController::graphGenerateGraph() {
  auto comparator = static_cast<HistogramBasedFrameComparator*>(
      detector_->frame_comparator());

  QVector<double> x(graph_data_.size());
  for (int i = 0; i < (int)graph_data_.size(); ++i)
    x[i] = i;

  widget_->graph(GraphLayer::DIFFERENCE)->setData(x, graph_data_);

  QVector<double> threshold_x(2),
      threshold_y(2, comparator->getParameters().histogramThreshold);
  threshold_x[0] = 0;
  threshold_x[1] = graph_data_.size() - 1;

  widget_->graph(GraphLayer::THRESHOLD)->setData(threshold_x, threshold_y);
  widget_->graph(GraphLayer::THRESHOLD)->setPen(QPen(Qt::red));

  widget_->xAxis->setLabel("frame no");
  widget_->yAxis->setLabel("similarity");

  widget_->xAxis->setRange(0, graph_data_.size() - 1);
  widget_->yAxis->setRange(0, 1.0);
  widget_->yAxis->axisRect()->setMinimumMargins(
      QMargins(0, cCurrentIndexLabelHeight, cGraphRightMargin, 0));

  emit graphNeedsUpdate(QCustomPlot::RefreshPriority::rpHint);
}
