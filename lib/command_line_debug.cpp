#include <command_line_debug.hpp>

#include <cstdio>

void CommandLineDebug::OnSceneDetected(SceneDetector::Frame& lastFrame,
									   SceneDetector::Frame& currentFrame) {
	printf("Scene! [%d;%d]\n", lastFrame.index_, currentFrame.index_);
}

void CommandLineDebug::OnDifferenceCalculated(SceneDetector::Frame& lastFrame,
											  SceneDetector::Frame& currentFrame,
											  double difference) {
	printf("[%d;%d] difference %f\n", lastFrame.index_, currentFrame.index_, difference);
}

