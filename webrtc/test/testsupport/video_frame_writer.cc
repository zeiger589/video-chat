/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "test/testsupport/video_frame_writer.h"

#include <cmath>
#include <cstdlib>
#include <limits>
#include <utility>

#include "absl/memory/memory.h"
#include "api/scoped_refptr.h"
#include "api/video/i420_buffer.h"
#include "common_video/libyuv/include/webrtc_libyuv.h"
#include "rtc_base/logging.h"

namespace webrtc {
namespace test {

VideoFrameWriter::VideoFrameWriter(std::string output_file_name,
                                   int width,
                                   int height,
                                   int fps)
    // We will move string here to prevent extra copy. We won't use const ref
    // to not corrupt caller variable with move and don't assume that caller's
    // variable won't be destructed before writer.
    : output_file_name_(std::move(output_file_name)),
      width_(width),
      height_(height),
      fps_(fps),
      frame_writer_(absl::make_unique<Y4mFrameWriterImpl>(output_file_name_,
                                                          width_,
                                                          height_,
                                                          fps_)) {
  // Init underlying frame writer and ensure that it is operational.
  RTC_CHECK(frame_writer_->Init());
}
VideoFrameWriter::~VideoFrameWriter() = default;

bool VideoFrameWriter::WriteFrame(const webrtc::VideoFrame& frame) {
  rtc::Buffer frame_buffer = ExtractI420BufferWithSize(frame, width_, height_);
  RTC_CHECK_EQ(frame_buffer.size(), frame_writer_->FrameLength());
  return frame_writer_->WriteFrame(frame_buffer.data());
}

void VideoFrameWriter::Close() {
  frame_writer_->Close();
}

rtc::Buffer VideoFrameWriter::ExtractI420BufferWithSize(const VideoFrame& frame,
                                                        int width,
                                                        int height) {
  if (frame.width() != width || frame.height() != height) {
    RTC_CHECK_LE(std::abs(static_cast<double>(width) / height -
                          static_cast<double>(frame.width()) / frame.height()),
                 2 * std::numeric_limits<double>::epsilon());
    // Same aspect ratio, no cropping needed.
    rtc::scoped_refptr<I420Buffer> scaled(I420Buffer::Create(width, height));
    scaled->ScaleFrom(*frame.video_frame_buffer()->ToI420());

    size_t length =
        CalcBufferSize(VideoType::kI420, scaled->width(), scaled->height());
    rtc::Buffer buffer(length);
    RTC_CHECK_NE(ExtractBuffer(scaled, length, buffer.data()), -1);
    return buffer;
  }

  // No resize.
  size_t length =
      CalcBufferSize(VideoType::kI420, frame.width(), frame.height());
  rtc::Buffer buffer(length);
  RTC_CHECK_NE(ExtractBuffer(frame, length, buffer.data()), -1);
  return buffer;
}

}  // namespace test
}  // namespace webrtc
