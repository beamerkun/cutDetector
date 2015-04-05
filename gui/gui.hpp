#pragma once

#include <memory>

#include <cut_detector.hpp>

namespace gui {
extern void startGraphicsInterface(int& argc,
                                   char* argv[],
                                   CutDetector* detector);
}
