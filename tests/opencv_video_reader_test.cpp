#include "gmock/gmock.h"

#include "video_reader.h"

#include "open_cv_video_reader.hpp"

using testing::_;

class MockVideoReaderObserver : public VideoReader::Observer {
 public:
  MOCK_METHOD2(onCurrentFrameChanged, void(cv::Mat& frame, int index));
  MOCK_METHOD1(onFileOpened, void(std::string filename));
  MOCK_METHOD0(onFileClosed, void());
};

class OpenCVVideoReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    video_reader_ = new OpenCVVideoReader();
    video_reader_->RegisterObserver(&mock_obs_);
  }

  virtual void TearDown() { delete video_reader_; }

  VideoReader* video_reader_;
  testing::StrictMock<MockVideoReaderObserver> mock_obs_;
  std::string h264_filename_ = "res/test.mp4";
};

class OpenCVVideoReaderH264Test : public OpenCVVideoReaderTest {
 protected:
  virtual void SetUp() {
    OpenCVVideoReaderTest::SetUp();
    EXPECT_CALL(mock_obs_, onFileOpened(h264_filename_));
    EXPECT_TRUE(video_reader_->openFile(h264_filename_));
  }
};

TEST_F(OpenCVVideoReaderTest, IsNotOpenOnStart) {
  EXPECT_FALSE(video_reader_->isOpen());
}

TEST_F(OpenCVVideoReaderTest, IsNotOpenAfterFailedOpen) {
  EXPECT_FALSE(video_reader_->isOpen());
  EXPECT_FALSE(video_reader_->openFile(""));
  EXPECT_FALSE(video_reader_->isOpen());
}

TEST_F(OpenCVVideoReaderTest, DontNotifyOnClosingIfClosed) {
  video_reader_->closeFile();
}

TEST_F(OpenCVVideoReaderTest, CanOpenH264) {
  EXPECT_FALSE(video_reader_->isOpen());
  EXPECT_CALL(mock_obs_, onFileOpened(h264_filename_));
  EXPECT_TRUE(video_reader_->openFile(h264_filename_));
  EXPECT_TRUE(video_reader_->isOpen());
  EXPECT_EQ(video_reader_->getCurrentFilename(), h264_filename_);
}

TEST_F(OpenCVVideoReaderTest, CanCloseH264) {
  EXPECT_CALL(mock_obs_, onFileOpened(h264_filename_));
  EXPECT_TRUE(video_reader_->openFile(h264_filename_));
  EXPECT_CALL(mock_obs_, onFileClosed());
  video_reader_->closeFile();
  EXPECT_FALSE(video_reader_->isOpen());
}

TEST_F(OpenCVVideoReaderH264Test, CanGetFrame) {
  cv::Mat result;
  EXPECT_CALL(mock_obs_, onCurrentFrameChanged(_, 0));
  EXPECT_TRUE(video_reader_->getFrame(0, result));
  EXPECT_EQ(video_reader_->getCurrentFrameIndex(), 0);
  EXPECT_NE(video_reader_->getTotalFrameCount(), 0);
  EXPECT_NE(video_reader_->getFrameHeight(), 0);
  EXPECT_NE(video_reader_->getFrameWidth(), 0);
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

TEST_F(OpenCVVideoReaderH264Test, CanGetSameFrameTwice) {
  cv::Mat result, second_result;
  EXPECT_CALL(mock_obs_, onCurrentFrameChanged(_, 0)).Times(1);
  EXPECT_TRUE(video_reader_->getFrame(0, result));
  EXPECT_EQ(video_reader_->getCurrentFrameIndex(), 0);

  EXPECT_CALL(mock_obs_, onCurrentFrameChanged(_, 10)).Times(1);
  EXPECT_TRUE(video_reader_->getFrame(10, second_result));
  EXPECT_EQ(video_reader_->getCurrentFrameIndex(), 10);

  EXPECT_CALL(mock_obs_, onCurrentFrameChanged(_, 0)).Times(1);
  EXPECT_TRUE(video_reader_->getFrame(0, second_result));
  EXPECT_EQ(video_reader_->getCurrentFrameIndex(), 0);
  EXPECT_TRUE(areMatSame(result, second_result));
}
