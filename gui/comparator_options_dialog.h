#ifndef COMPARATOR_OPTIONS_DIALOG_H
#define COMPARATOR_OPTIONS_DIALOG_H

#include <QDialog>

namespace Ui {
class ComparatorOptionsDialog;
}

class ComparatorOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComparatorOptionsDialog(QWidget *parent = 0);
    ~ComparatorOptionsDialog();

private:
    Ui::ComparatorOptionsDialog *ui;
};

#endif // COMPARATOR_OPTIONS_DIALOG_H
