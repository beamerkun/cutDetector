#include <command_line.h>

#include <memory>

#include <tclap/CmdLine.h>

class CommandLineImpl : public CommandLine {
  public:
	CommandLineImpl();
	bool Init(int argc, char *argv[]) override;
	bool isDebug() override;
	std::string getFilename() override;
	std::string getOptionString() override;

  private:
	TCLAP::CmdLine cmdLine_;
	std::unique_ptr<TCLAP::UnlabeledValueArg<std::string>> filenameArg_;
	std::unique_ptr<TCLAP::SwitchArg> debugSwitch_;
	std::unique_ptr<TCLAP::ValueArg<std::string>> optionsArg_;
}; // CommandLineImpl
