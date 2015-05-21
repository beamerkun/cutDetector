#include <histogram_based_frame_comparator.hpp>  // filter interface

#include <fstream>  // filestream
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures
#include <opencv2/imgproc/imgproc.hpp>  // Drawing rectangles
#include <rapidjson/document.h>         // JSON parsing
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <frame_comparator_strings.hpp>

using namespace cv;

/*
namespace {
void colorSubspaces(FrameComparatorImpl::param param,
                    cv::Mat& frame1,
                    cv::Mat& frame2,
                    cv::Mat& result) {
  bool limit = param.limitRejects;
  int N = param.rejected;
  int WIDTH_DIV = param.widthDiv;
  int HEIGHT_DIV = param.heightDiv;
  int SUBSPACES = WIDTH_DIV * HEIGHT_DIV;
  Size frameSize = frame1.size();
  int hDiv = frameSize.height / HEIGHT_DIV;
  int wDiv = frameSize.width / WIDTH_DIV;
  double m = mean(result)[0];
  for (int h = 0; h < HEIGHT_DIV; ++h) {
    for (int w = 0; w < WIDTH_DIV; ++w) {
      double* ptr = result.ptr<double>(h * WIDTH_DIV + w);
      if (std::abs(*ptr - m) < 0.3 * m && false) {
        Rect region = Rect(w * wDiv, h * hDiv, wDiv, hDiv);
        rectangle(frame1, region, Scalar(0, 0, 255, 0.3), CV_FILLED);
      }
    }
  }
}
}
*/

void HistogramBasedFrameComparator::setOptionsFilename(std::string optionsFilename) {
  using namespace rapidjson;

  std::vector<char> contents;

  std::ifstream file(optionsFilename);

  if (file) {
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);

    contents.reserve(static_cast<unsigned long>(length));
    file.read(&contents[0], length);
  }

  Document config;
  config.Parse(&contents[0]);

  if (config.HasMember(kParameterHistogramThreshold)) {
    parameters.histogramThreshold =
        config[kParameterHistogramThreshold].GetDouble();
  }
  if (config.HasMember(kParameterLimitRejects)) {
    parameters.limitRejects = config[kParameterLimitRejects].GetBool();
  }
  if (config.HasMember(kParameterRejected)) {
    parameters.rejected = config[kParameterRejected].GetInt();
  }
  if (config.HasMember(kParameterWidthDiv)) {
    parameters.widthDiv = config[kParameterWidthDiv].GetInt();
  }
  if (config.HasMember(kParameterHeightDiv)) {
    parameters.heightDiv = config[kParameterHeightDiv].GetInt();
  }
}

bool HistogramBasedFrameComparator::isDifferentScene(Mat& lastFrame,
                                           Mat& currentFrame,
                                           double* distance) {
  int widthDiv = parameters.widthDiv;
  int heightDiv = parameters.heightDiv;
  int subspaces = widthDiv * heightDiv;
  double histogramDistance;

  // Divide frames into HEIGHT_DIV*WIDTH_DIV segments.
  Mat result;
  Size frameSize = lastFrame.size();
  int hDiv = frameSize.height / heightDiv;
  int wDiv = frameSize.width / widthDiv;
  for (int h = 0; h < heightDiv; ++h) {
    for (int w = 0; w < widthDiv; ++w) {
      Rect region = Rect(w * wDiv, h * hDiv, wDiv, hDiv);
      Mat r1 = lastFrame(region);
      Mat r2 = currentFrame(region);
      result.push_back(calculateFrameDistance(r1, r2));
    }
  }

  histogramDistance = 0;
  int count = 0;
  int rejected = 0;
  double m = mean(result)[0];
  for (int i = 0; i < subspaces; ++i) {
    double* ptr = result.ptr<double>(i);
    if (std::abs(*ptr - m) < 0.3 * m ||
        (parameters.limitRejects && parameters.rejected < rejected)) {
      histogramDistance += *ptr;
      count++;
    } else {
      rejected++;
    }
  }
  *distance = histogramDistance / count;

  return histogramDistance / count < parameters.histogramThreshold;
}

double HistogramBasedFrameComparator::calculateFrameDistance(Mat& lastFrame,
                                                   Mat& currentFrame) {
  Mat channelsLastFrame[3];
  Mat channelsCurrentFrame[3];

  // divide channels
  split(lastFrame, channelsLastFrame);
  split(currentFrame, channelsCurrentFrame);

  /* Histogram calculation */

  // Levels
  int bins = 80;
  int histSize[] = {bins};
  // Brightness range
  float lranges[] = {0, 60};
  const float* ranges[] = {lranges};
  MatND histogramCurrent;
  MatND histogramLast;

  int channels[] = {0};

  double sum = 0;

  for (int i = 0; i < 3; ++i) {
    calcHist(channelsLastFrame + i, 1, channels, Mat(), histogramLast, 1,
             histSize, ranges, true, false);
    calcHist(channelsCurrentFrame + i, 1, channels, Mat(), histogramCurrent, 1,
             histSize, ranges, true, false);

    sum += compareHist(histogramCurrent, histogramLast, CV_COMP_CORREL);
  }

  return sum / 3;
}

std::string HistogramBasedFrameComparator::getJsonFile() {
  using namespace rapidjson;

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key(kParameterHistogramThreshold);
  writer.Double(parameters.histogramThreshold);
  writer.Key(kParameterLimitRejects);
  writer.Bool(parameters.limitRejects);
  writer.Key(kParameterRejected);
  writer.Int(parameters.rejected);
  writer.Key(kParameterWidthDiv);
  writer.Int(parameters.widthDiv);
  writer.Key(kParameterHeightDiv);
  writer.Int(parameters.heightDiv);
  writer.EndObject();

  return std::string(buffer.GetString());
}
