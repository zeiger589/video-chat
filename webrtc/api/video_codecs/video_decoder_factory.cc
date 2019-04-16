/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/video_codecs/video_decoder_factory.h"

#include "api/video_codecs/video_decoder.h"

namespace webrtc {

std::unique_ptr<VideoDecoder> VideoDecoderFactory::LegacyCreateVideoDecoder(
    const SdpVideoFormat& format,
    const std::string& receive_stream_id) {
  return CreateVideoDecoder(format);
}

}  // namespace webrtc
