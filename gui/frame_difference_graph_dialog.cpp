#include "frame_difference_graph_dialog.h"
#include "ui_frame_difference_graph_dialog.h"

frameDifferenceGraphDialog::frameDifferenceGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frameDifferenceGraphDialog)
{
    ui->setupUi(this);
}

frameDifferenceGraphDialog::~frameDifferenceGraphDialog()
{
    delete ui;
}
