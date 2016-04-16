#include "gmock/gmock.h"

#include <memory>

#include "video_reader.h"

#include "open_cv_video_reader.hpp"
#include "video_reader_mock.hpp"

using testing::_;

template <typename T>
class VideoReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    video_reader_.reset(new T());
    video_reader_->RegisterObserver(&mock_obs_);
  }

  virtual void TearDown() {}

  std::unique_ptr<VideoReader> video_reader_;
  testing::StrictMock<VideoReaderObserverMock> mock_obs_;
  std::string h264_filename_ = "res/test.mp4";
};

template <typename T>
class VideoReaderH264Test : public VideoReaderTest<T> {
 protected:
  virtual void SetUp() {
    VideoReaderTest<T>::SetUp();
    EXPECT_CALL(this->mock_obs_, onFileOpened(this->h264_filename_));
    EXPECT_TRUE(this->video_reader_->openFile(this->h264_filename_));
  }
};

typedef testing::Types<OpenCVVideoReader> TestTypes;
TYPED_TEST_CASE(VideoReaderTest, TestTypes);
TYPED_TEST_CASE(VideoReaderH264Test, TestTypes);

TYPED_TEST(VideoReaderTest, IsNotOpenOnStart) {
  EXPECT_FALSE(this->video_reader_->isOpen());
}

TYPED_TEST(VideoReaderTest, IsNotOpenAfterFailedOpen) {
  EXPECT_FALSE(this->video_reader_->isOpen());
  EXPECT_FALSE(this->video_reader_->openFile(""));
  EXPECT_FALSE(this->video_reader_->isOpen());
}

TYPED_TEST(VideoReaderTest, DontNotifyOnClosingIfClosed) {
  // Video reader observer is a strict mock, so we don't need to set more
  // expectations.
  this->video_reader_->closeFile();
}

TYPED_TEST(VideoReaderTest, CanOpenH264) {
  EXPECT_FALSE(this->video_reader_->isOpen());
  EXPECT_CALL(this->mock_obs_, onFileOpened(this->h264_filename_));
  EXPECT_TRUE(this->video_reader_->openFile(this->h264_filename_));
  EXPECT_TRUE(this->video_reader_->isOpen());
  EXPECT_EQ(this->video_reader_->getCurrentFilename(), this->h264_filename_);
}

TYPED_TEST(VideoReaderTest, CanCloseH264) {
  EXPECT_CALL(this->mock_obs_, onFileOpened(this->h264_filename_));
  EXPECT_TRUE(this->video_reader_->openFile(this->h264_filename_));
  EXPECT_CALL(this->mock_obs_, onFileClosed());
  this->video_reader_->closeFile();
  EXPECT_FALSE(this->video_reader_->isOpen());
}

TYPED_TEST(VideoReaderH264Test, CanGetFrame) {
  cv::Mat result;
  EXPECT_CALL(this->mock_obs_, onCurrentFrameChanged(_, 0));
  EXPECT_TRUE(this->video_reader_->getFrame(0, result));
  EXPECT_EQ(this->video_reader_->getCurrentFrameIndex(), 0);
  EXPECT_NE(this->video_reader_->getTotalFrameCount(), 0);
  EXPECT_NE(this->video_reader_->getFrameHeight(), 0);
  EXPECT_NE(this->video_reader_->getFrameWidth(), 0);
}

bool areMatSame(cv::Mat& mat1, cv::Mat& mat2) {
  if (mat1.empty() && mat2.empty())
    return true;

  // if dimensionality of two mat is not identical, these two mat is not
  // identical
  if (mat1.cols != mat2.cols || mat1.rows != mat2.rows ||
      mat1.dims != mat2.dims || mat1.channels() != mat2.channels()) {
    return false;
  }

  std::vector<cv::Mat> mat1_channels(mat1.channels());
  std::vector<cv::Mat> mat2_channels(mat2.channels());
  cv::Mat diff;

  for (int i = 0; i < mat1.channels(); ++i) {
    cv::compare(mat1_channels[i], mat2_channels[i], diff, cv::CMP_NE);
    int nz = cv::countNonZero(diff);
    if (nz != 0)
      return false;
  }
  return true;
}

TYPED_TEST(VideoReaderH264Test, CanGetSameFrameTwice) {
  cv::Mat result, second_result;
  EXPECT_CALL(this->mock_obs_, onCurrentFrameChanged(_, 0)).Times(1);
  EXPECT_TRUE(this->video_reader_->getFrame(0, result));
  EXPECT_EQ(this->video_reader_->getCurrentFrameIndex(), 0);

  EXPECT_CALL(this->mock_obs_, onCurrentFrameChanged(_, 10)).Times(1);
  EXPECT_TRUE(this->video_reader_->getFrame(10, second_result));
  EXPECT_EQ(this->video_reader_->getCurrentFrameIndex(), 10);

  EXPECT_CALL(this->mock_obs_, onCurrentFrameChanged(_, 0)).Times(1);
  EXPECT_TRUE(this->video_reader_->getFrame(0, second_result));
  EXPECT_EQ(this->video_reader_->getCurrentFrameIndex(), 0);
  EXPECT_TRUE(areMatSame(result, second_result));
}
