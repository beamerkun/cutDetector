// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#ifndef COMPARATOR_OPTIONS_DIALOG_H
#define COMPARATOR_OPTIONS_DIALOG_H

#include <QDialog>

#include <histogram_based_frame_comparator.hpp>

namespace Ui {
class ComparatorOptionsDialog;
}

class ComparatorOptionsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ComparatorOptionsDialog(QWidget* parent = 0,
                                   HistogramBasedFrameComparator* frame_comparator = 0);
  ~ComparatorOptionsDialog();

  void saveParameters();
  void applyParameters();
  void loadParameters();

 private:
  Ui::ComparatorOptionsDialog* ui;
  HistogramBasedFrameComparator* frame_comparator_;
};

#endif  // COMPARATOR_OPTIONS_DIALOG_H
