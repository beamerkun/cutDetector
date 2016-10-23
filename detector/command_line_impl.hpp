# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
#pragma once

#include <command_line.h>

#include <memory>

#include <tclap/CmdLine.h>

class CommandLineImpl : public CommandLine {
 public:
  CommandLineImpl();

  bool Init(int argc, char* argv[]) override;
  bool isDebug() override;
  std::string getFilename() override;
  std::string getOptionsFilename() override;

 private:
  TCLAP::CmdLine cmdLine_;
  std::unique_ptr<TCLAP::UnlabeledValueArg<std::string>> filenameArg_;
  std::unique_ptr<TCLAP::SwitchArg> debugSwitch_;
  std::unique_ptr<TCLAP::ValueArg<std::string>> optionsArg_;
};  // CommandLineImpl
