#ifndef FRAME_DIFFERENCE_GRAPH_DIALOG_H
#define FRAME_DIFFERENCE_GRAPH_DIALOG_H

#include <vector>

#include <QDialog>

#include <histogram_based_frame_comparator.hpp>

namespace Ui {
class FrameDifferenceGraphDialog;
}

class FrameDifferenceGraphDialog : public QDialog {
  Q_OBJECT

 public:
  explicit FrameDifferenceGraphDialog(QWidget* parent = 0);
  ~FrameDifferenceGraphDialog();

  void clearData() { data_.clear(); }
  void setComparator(HistogramBasedFrameComparator* comparator) {
    comparator_ = comparator;
  }
  void limitData(int limit);
  void setCuts(QList<QPair<int,int>> scenes);

 public slots:
  void addDifferenceValue(int frameIndex, double difference);

 private:
  void generateGraph();

  Ui::FrameDifferenceGraphDialog* ui;

  HistogramBasedFrameComparator* comparator_;
  QVector<double> data_;
  QVector<double> cuts_;
};

#endif  // FRAME_DIFFERENCE_GRAPH_DIALOG_H
