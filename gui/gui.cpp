#include <gui.hpp>

#include <memory>

#include <QApplication>

#include <main_window.h>

bool gui::startGraphicsInterface(int& argc, char* argv[]) {
  std::unique_ptr<QApplication> app(new QApplication(argc, argv));
  main_window main;
  main.show();
  while (true)
    ;
}
