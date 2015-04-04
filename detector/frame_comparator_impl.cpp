#include <frame_comparator_impl.hpp> // filter interface

#include <string>
#include <sstream>  // string to number conversion
#include <fstream>  // filestream
#include <vector>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures
#include <opencv2/imgproc/imgproc.hpp>  // Drawing rectangles

#include <rapidjson/document.h>

using namespace cv;

namespace {
  void colorSubspaces(FrameComparatorImpl::param param, cv::Mat& frame1, cv::Mat& frame2, cv::Mat& result) {
    bool limit = param.limitRejects;
    int N = param.rejected;
    int WIDTH_DIV = param.widthDiv;
    int HEIGHT_DIV = param.heightDiv;
    int SUBSPACES = WIDTH_DIV * HEIGHT_DIV;
    Size frameSize = frame1.size();
    int hDiv = frameSize.height / HEIGHT_DIV;
    int wDiv = frameSize.width / WIDTH_DIV;
    double m = mean(result)[0];
    for( int h = 0; h < HEIGHT_DIV; ++h ){
      for( int w = 0; w < WIDTH_DIV; ++w ){
        double* ptr = result.ptr<double>(h*WIDTH_DIV+w); 
        if( abs(*ptr - m) < 0.3*m && false) {
          Rect region = Rect( w*wDiv, h*hDiv, wDiv, hDiv );
          rectangle(frame1, region, Scalar(0,0,255,0.3), CV_FILLED);
        }
      }
    }
  }
}

void FrameComparatorImpl::setOptionsFilename(std::string optionsFilename) {
  using namespace rapidjson;
  
  std::vector<char> contents;
  
  std::ifstream file(optionsFilename);
      
  if(file) {
      file.seekg(0, std::ios::end);
      std::streampos length = file.tellg();
      file.seekg(0, std::ios::beg);
        
      contents.reserve(length);
      file.read(&contents[0], length);
  }

  Document config;
  config.Parse(&contents[0]);

  // TODO refactor out strings
  if(config.HasMember("histogram_threshold")) {
      parameters.histogramThreshold = config["histogram_treshold"].GetDouble();
  }
  if(config.HasMember("limit_rejects")) {
      parameters.limitRejects = config["limit_rejects"].GetBool();
  }
  if(config.HasMember("rejected")) {
      parameters.rejected = config["rejected"].GetInt();
  }
  if(config.HasMember("width_div")) {
      parameters.rejected = config["width_div"].GetInt();
  }
  if(config.HasMember("height_div")) {
      parameters.rejected = config["height_div"].GetInt();
  }
}

bool FrameComparatorImpl::isDifferentScene(Mat& lastFrame, Mat& currentFrame, double* distance){
  int WIDTH_DIV = parameters.widthDiv;
  int HEIGHT_DIV = parameters.heightDiv;
  int SUBSPACES = WIDTH_DIV * HEIGHT_DIV;
  double histogramDistance;

  // Divide frames into HEIGHT_DIV*WIDTH_DIV segments.
  Mat result;
  Size frameSize = lastFrame.size();
  int hDiv = frameSize.height / HEIGHT_DIV;
  int wDiv = frameSize.width / WIDTH_DIV;
  for( int h = 0; h < HEIGHT_DIV; ++h ){
    for( int w = 0; w < WIDTH_DIV; ++w ){
      Rect region = Rect( w*wDiv, h*hDiv, wDiv, hDiv );
      Mat r1 = lastFrame(region);
      Mat r2 = currentFrame(region);
      result.push_back( calculateFrameDistance(r1, r2) );
    }
  }

  histogramDistance = 0;
  int count = 0;
  double m = mean(result)[0];
  for( int i = 0; i < SUBSPACES; ++i ){
    double* ptr = result.ptr<double>(i); 
    if (abs(*ptr - m) < 0.3*m) {
      histogramDistance += *ptr;
      count++;
    }
  }
  *distance = histogramDistance/count;

  return histogramDistance/count < parameters.histogramThreshold;
}

double FrameComparatorImpl::calculateFrameDistance(Mat& lastFrame, Mat& currentFrame){
  Mat channelsLastFrame[3];
  Mat channelsCurrentFrame[3];

  // podział klatki na 3 oddzielne kanały
  split(lastFrame   , channelsLastFrame);
  split(currentFrame  , channelsCurrentFrame);

  ///////// OBLICZANIE HISTOGRAMU //////
  // ilosc poziomow histogrami
  int bins = 80;
  int histSize[] = {bins};
  // zakres wartosci jasnosci
  float lranges[] = { 0, 60 };
  const float* ranges[] = { lranges };
  MatND histogramCurrent;
  MatND histogramLast;

  int channels[] = {0};

  double sum = 0;

  for( int i = 0; i < 3; ++i ){
    calcHist(channelsLastFrame + i, 1, channels, Mat(),
        histogramLast, 1, histSize, ranges,
        true, false );
    calcHist(channelsCurrentFrame + i, 1, channels, Mat(),
        histogramCurrent, 1, histSize, ranges,
        true, false );

    sum += compareHist(histogramCurrent,
        histogramLast,
        CV_COMP_CORREL); 
  }

  return sum/3;
}

