// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include "comparator_options_dialog.h"
#include "ui_comparator_options_dialog.h"

ComparatorOptionsDialog::ComparatorOptionsDialog(
    QWidget* parent,
    HistogramBasedFrameComparator* frame_comparator)
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
  QObject::connect(ui->rejectedLimitCheckBox, &QCheckBox::toggled,
                   ui->coefficientLineEdit, &QLineEdit::setEnabled);
  QObject::connect(ui->rejectedLimitCheckBox, &QCheckBox::toggled,
                   ui->constantLineEdit, &QLineEdit::setEnabled);
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
  HistogramBasedFrameComparator::param parameters;
  parameters.histogramThreshold = (double)ui->thresholdSlider->value() / 1000.0;
  parameters.heightDiv = ui->heightDivLineEdit->text().toInt();
  parameters.widthDiv = ui->widthDivLineEdit->text().toInt();
  parameters.limitRejects = ui->rejectedLimitCheckBox->isChecked();
  parameters.rejected = ui->rejectedLineEdit->text().toInt();
  parameters.coefficient = ui->coefficientLineEdit->text().toFloat();
  parameters.constant = ui->constantLineEdit->text().toFloat();
  frame_comparator_->setParameters(parameters);
}

void ComparatorOptionsDialog::loadParameters() {
  HistogramBasedFrameComparator::param parameters = frame_comparator_->getParameters();
  ui->thresholdSlider->setValue(parameters.histogramThreshold * 1000);
  ui->heightDivLineEdit->setText(QString::number(parameters.heightDiv));
  ui->widthDivLineEdit->setText(QString::number(parameters.widthDiv));
  ui->rejectedLimitCheckBox->setChecked(parameters.limitRejects);
  ui->rejectedLineEdit->setText(QString::number(parameters.rejected));
  ui->coefficientLineEdit->setText(QString::number(parameters.coefficient));
  ui->constantLineEdit->setText(QString::number(parameters.constant));
}
