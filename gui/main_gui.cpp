#include <memory>

#include <QApplication>

#include <cut_detector.hpp>

#include <main_window.h>

int main(int argc, char* argv[]) {
  std::unique_ptr<QApplication> app(new QApplication(argc, argv));
  main_window main(nullptr, new CutDetector());
  main.show();
  app->exec();
  return 0;
}
