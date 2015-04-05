#include <gui.hpp>

#include <memory>

#include <QApplication>

#include <main_window.h>

void gui::startGraphicsInterface(int& argc, char* argv[], CutDetector* detector) {
  std::unique_ptr<QApplication> app(new QApplication(argc, argv));
  main_window main(nullptr, detector);
  main.show();
  app->exec();
}
