// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#pragma once

#include <cut_detector.hpp>

#include <qcustomplot.h>

class GraphWidgetController : public QObject {
 Q_OBJECT
 public:
  enum GraphLayer {
    DIFFERENCE = 0,
    THRESHOLD = 1,
    CURRENT_FRAME = 2,
  };

  explicit GraphWidgetController(QCustomPlot* widget, CutDetector* detector);

 public slots:
  void graphClearData() { graph_data_.clear(); }
  void graphLimitData(int limit);
  void graphSetCurrentFrame(int index);
  void graphSetCuts(QList<QPair<int, int>> scenes);

 signals:
  void graphNeedsUpdate(QCustomPlot::RefreshPriority priority);

 public slots:
  void graphAddDifferenceValue(int frameIndex, double difference);

 public:
  void graphGenerateGraph();

 private:
  QCustomPlot* widget_;
  CutDetector* detector_;

  QVector<double> graph_data_;
  QVector<double> graph_cuts_;
};
