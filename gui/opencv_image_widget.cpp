// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#include "opencv_image_widget.h"

#include <QPainter>

OpenCVImageWidget::OpenCVImageWidget(QWidget* parent) : QWidget(parent) {
}

OpenCVImageWidget::~OpenCVImageWidget() {
}

void OpenCVImageWidget::showImage(const cv::Mat image) {
  // Convert the image to the RGB888 format
  switch (image.type()) {
    case CV_8UC1:
      cvtColor(image, _tmp, CV_GRAY2RGB);
      break;
    case CV_8UC3:
      cvtColor(image, _tmp, CV_BGR2RGB);
      break;
  }

  // QImage needs the data to be stored continuously in memory
  assert(_tmp.isContinuous());
  convertMat();

  repaint();
}

void OpenCVImageWidget::paintEvent(QPaintEvent* /*event*/) {
  int x = (this->width() - _qimage.width()) / 2;
  int y = (this->height() - _qimage.height()) / 2;

  QPainter painter(this);
  painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
  painter.drawImage(QPoint(x, y), _qimage);
  painter.end();
}

void OpenCVImageWidget::resizeEvent(QResizeEvent* /*event*/) {
  convertMat();
}

void OpenCVImageWidget::convertMat() {
  _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols * 3,
                   QImage::Format_RGB888);
  if (!_qimage.isNull())
    _qimage =
        _qimage.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
}
