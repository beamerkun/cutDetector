#include "comparator_options_dialog.h"
#include "ui_comparator_options_dialog.h"

ComparatorOptionsDialog::ComparatorOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComparatorOptionsDialog)
{
    ui->setupUi(this);
}

ComparatorOptionsDialog::~ComparatorOptionsDialog()
{
    delete ui;
}
