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
