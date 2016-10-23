// Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
// Licensed under MIT License
// Full text avaliable in LICENSE file located in project root
// http://github.com/beamerkun/cutDetector/blob/master/LICENSE

#pragma once

#include <string>

class CommandLine {
 public:
  virtual ~CommandLine() {}

  virtual bool Init(int argc, char* argv[]) = 0;
  virtual bool isDebug() = 0;
  virtual std::string getFilename() = 0;
  virtual std::string getOptionsFilename() = 0;
};  // CommandLine
