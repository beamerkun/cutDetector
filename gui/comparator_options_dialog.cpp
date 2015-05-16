#include "comparator_options_dialog.h"
#include "ui_comparator_options_dialog.h"

ComparatorOptionsDialog::ComparatorOptionsDialog(
    QWidget* parent,
    FrameComparatorImpl* frame_comparator)
    : QDialog(parent),
      ui(new Ui::ComparatorOptionsDialog),
      frame_comparator_(frame_comparator) {
  ui->setupUi(this);

  // CONNECTS
  // Threshold
  QObject::connect(ui->thresholdSlider, &QSlider::valueChanged, [=](int value) {
    double fp_value = (double)value / 1000.0;
    ui->thresholdValueLabel->setText(QString::number(fp_value));
  });
  // Rejects
  QObject::connect(ui->rejectedLimitCheckBox, &QCheckBox::toggled,
                   ui->rejectedLineEdit, &QLineEdit::setEnabled);
  // Buttons
  QObject::connect(ui->applyButton, &QAbstractButton::clicked, [=]() {
    applyParameters();
    close();
  });

  loadParameters();
}

ComparatorOptionsDialog::~ComparatorOptionsDialog() {
  delete ui;
}

void ComparatorOptionsDialog::applyParameters() {
  FrameComparatorImpl::param parameters;
  parameters.histogramThreshold = (double)ui->thresholdSlider->value() / 1000.0;
  parameters.heightDiv = ui->heightDivLineEdit->text().toInt();
  parameters.widthDiv = ui->widthDivLineEdit->text().toInt();
  parameters.limitRejects = ui->rejectedLimitCheckBox->isChecked();
  parameters.rejected = ui->rejectedLineEdit->text().toInt();
}

void ComparatorOptionsDialog::loadParameters() {
  FrameComparatorImpl::param parameters = frame_comparator_->getParameters();
  ui->thresholdSlider->setValue(parameters.histogramThreshold * 1000);
  ui->heightDivLineEdit->setText(QString::number(parameters.heightDiv));
  ui->widthDivLineEdit->setText(QString::number(parameters.widthDiv));
  ui->rejectedLimitCheckBox->setChecked(parameters.limitRejects);
  ui->rejectedLineEdit->setText(QString::number(parameters.rejected));
}
