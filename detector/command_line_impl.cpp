#include <command_line_impl.hpp>

#include <tclap/CmdLine.h>

// TODO Refactor out strings
CommandLineImpl::CommandLineImpl() :
  cmdLine_("cutDetector", ' ', "0.1") {
  filenameArg_.reset(new TCLAP::UnlabeledValueArg<std::string>
      ("name", "filename", true, "input.mp4", "string", cmdLine_));
  debugSwitch_.reset(new TCLAP::SwitchArg("g", "debug", "enable debug mode", cmdLine_, false));
  optionsArg_.reset(new TCLAP::ValueArg<std::string>
      ("o", "option_string", "options for frame comparator", false,"","string",cmdLine_));
}

bool CommandLineImpl::Init(int argc, char *argv[]) {
  try {
    cmdLine_.parse(argc, argv);
  } 
  catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return false;
  }
  return true;
}

bool CommandLineImpl::isDebug() {
  return debugSwitch_->getValue();
}

std::string CommandLineImpl::getFilename() {
  return filenameArg_->getValue();
}

std::string CommandLineImpl::getOptionString() {
  return optionsArg_->getValue();
}

