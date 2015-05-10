#ifndef OPENCV_IMAGE_WIDGET_H
#define OPENCV_IMAGE_WIDGET_H

// Source:
// http://develnoter.blogspot.com/2012/05/integrating-opencv-in-qt-gui.html

#include <QObject>
#include <QWidget>

#include <opencv2/opencv.hpp>

class OpenCVImageWidget : public QWidget {
  Q_OBJECT
 public:
  explicit OpenCVImageWidget(QWidget* parent = 0);
  ~OpenCVImageWidget();

  QSize sizeHint() const { return _qimage.size(); }
  QSize minimumSizeHint() const { return _qimage.size(); }

 public slots:
  void showImage(const cv::Mat image);

 protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

 private:
  void convertMat();

  QImage _qimage;
  cv::Mat _tmp;
};

#endif  // OPENCV_IMAGE_WIDGET_H
