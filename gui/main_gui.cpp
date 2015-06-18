#include <cut_detector.hpp>
#include <gui.hpp>

int main(int argc, char* argv[]) {
  std::unique_ptr<CutDetector> detector(new CutDetector());
  gui::startGraphicsInterface(argc, argv, detector.release());
  return 0;
}
