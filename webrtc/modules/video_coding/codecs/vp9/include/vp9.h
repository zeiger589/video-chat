/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 *
 */

#ifndef MODULES_VIDEO_CODING_CODECS_VP9_INCLUDE_VP9_H_
#define MODULES_VIDEO_CODING_CODECS_VP9_INCLUDE_VP9_H_

#include <memory>
#include <vector>

#include "api/video_codecs/sdp_video_format.h"
#include "media/base/codec.h"
#include "modules/video_coding/include/video_codec_interface.h"

namespace webrtc {

// Returns a vector with all supported internal VP9 profiles that we can
// negotiate in SDP, in order of preference.
std::vector<SdpVideoFormat> SupportedVP9Codecs();

class VP9Encoder : public VideoEncoder {
 public:
  // Deprecated. Returns default implementation using VP9 Profile 0.
  // TODO(emircan): Remove once this is no longer used.
  static std::unique_ptr<VP9Encoder> Create();
  // Parses VP9 Profile from |codec| and returns the appropriate implementation.
  static std::unique_ptr<VP9Encoder> Create(const cricket::VideoCodec& codec);

  ~VP9Encoder() override {}
};

class VP9Decoder : public VideoDecoder {
 public:
  static std::unique_ptr<VP9Decoder> Create();

  ~VP9Decoder() override {}
};
}  // namespace webrtc

#endif  // MODULES_VIDEO_CODING_CODECS_VP9_INCLUDE_VP9_H_
