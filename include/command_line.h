#pragma once

#include <string>

class CommandLine {
 public:
  virtual bool Init(int argc, char* argv[]) = 0;
  virtual bool isDebug() = 0;
  virtual bool isGui() = 0;
  virtual std::string getFilename() = 0;
  virtual std::string getOptionsFilename() = 0;
};  // CommandLine
