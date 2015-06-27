#ifndef FRAME_DIFFERENCE_GRAPH_DIALOG_H
#define FRAME_DIFFERENCE_GRAPH_DIALOG_H

#include <QDialog>

namespace Ui {
class frameDifferenceGraphDialog;
}

class frameDifferenceGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit frameDifferenceGraphDialog(QWidget *parent = 0);
    ~frameDifferenceGraphDialog();

private:
    Ui::frameDifferenceGraphDialog *ui;
};

#endif // FRAME_DIFFERENCE_GRAPH_DIALOG_H
