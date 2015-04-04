#include <command_line_impl.hpp>

#include <tclap/CmdLine.h>

#include <command_line_strings.hpp>

CommandLineImpl::CommandLineImpl() :
    cmdLine_(kProgramName, ' ', kProgramVersion) {
  filenameArg_.reset(new TCLAP::UnlabeledValueArg<std::string>(
                         kParameterFilenameName,
                         kParameterFilenameDescription,
                         true,
                         kParameterFilenameDefault,
                         "string",
                         cmdLine_));
  debugSwitch_.reset(new TCLAP::SwitchArg(
                         kParameterDebugSwitch,
                         kParameterDebugName,
                         kParameterDebugDescription,
                         cmdLine_,
                         false));
  optionsArg_.reset(new TCLAP::ValueArg<std::string>(
                        kParameterOptionsFilenameSwitch,
                        kParameterOptionsFilenameName,
                        kParameterOptionsFilenameDescription,
                        false,
                        kParameterOptionsFilenameDefault,
                        "string",
                        cmdLine_));
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

std::string CommandLineImpl::getOptionsFilename() {
  return optionsArg_->getValue();
}
