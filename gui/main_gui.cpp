// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

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
