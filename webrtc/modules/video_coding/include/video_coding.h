/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_VIDEO_CODING_INCLUDE_VIDEO_CODING_H_
#define MODULES_VIDEO_CODING_INCLUDE_VIDEO_CODING_H_

#include "api/fec_controller.h"
#include "api/video/video_frame.h"
#include "api/video_codecs/video_codec.h"
#include "modules/include/module.h"
#include "modules/include/module_common_types.h"
#include "modules/video_coding/include/video_coding_defines.h"
#include "rtc_base/deprecation.h"

namespace webrtc {

class Clock;
class EncodedImageCallback;
class VideoDecoder;
class VideoEncoder;
struct CodecSpecificInfo;

class VideoCodingModule : public Module {
 public:
  // DEPRECATED.
  static VideoCodingModule* Create(Clock* clock);

  /*
   *   Receiver
   */

  // Register possible receive codecs, can be called multiple times for
  // different codecs.
  // The module will automatically switch between registered codecs depending on
  // the
  // payload type of incoming frames. The actual decoder will be created when
  // needed.
  //
  // Input:
  //      - receiveCodec      : Settings for the codec to be registered.
  //      - numberOfCores     : Number of CPU cores that the decoder is allowed
  //      to use.
  //      - requireKeyFrame   : Set this to true if you don't want any delta
  //      frames
  //                            to be decoded until the first key frame has been
  //                            decoded.
  //
  // Return value      : VCM_OK, on success.
  //                     < 0,    on error.
  virtual int32_t RegisterReceiveCodec(const VideoCodec* receiveCodec,
                                       int32_t numberOfCores,
                                       bool requireKeyFrame = false) = 0;

  // Register an external decoder object.
  //
  // Input:
  //      - externalDecoder : Decoder object to be used for decoding frames.
  //      - payloadType     : The payload type which this decoder is bound to.
  virtual void RegisterExternalDecoder(VideoDecoder* externalDecoder,
                                       uint8_t payloadType) = 0;

  // Register a receive callback. Will be called whenever there is a new frame
  // ready
  // for rendering.
  //
  // Input:
  //      - receiveCallback        : The callback object to be used by the
  //      module when a
  //                                 frame is ready for rendering.
  //                                 De-register with a NULL pointer.
  //
  // Return value      : VCM_OK, on success.
  //                     < 0,    on error.
  virtual int32_t RegisterReceiveCallback(
      VCMReceiveCallback* receiveCallback) = 0;

  // Register a frame type request callback. This callback will be called when
  // the
  // module needs to request specific frame types from the send side.
  //
  // Input:
  //      - frameTypeCallback      : The callback object to be used by the
  //      module when
  //                                 requesting a specific type of frame from
  //                                 the send side.
  //                                 De-register with a NULL pointer.
  //
  // Return value      : VCM_OK, on success.
  //                     < 0,    on error.
  virtual int32_t RegisterFrameTypeCallback(
      VCMFrameTypeCallback* frameTypeCallback) = 0;

  // Registers a callback which is called whenever the receive side of the VCM
  // encounters holes in the packet sequence and needs packets to be
  // retransmitted.
  //
  // Input:
  //              - callback      : The callback to be registered in the VCM.
  //
  // Return value     : VCM_OK,     on success.
  //                    <0,         on error.
  virtual int32_t RegisterPacketRequestCallback(
      VCMPacketRequestCallback* callback) = 0;

  // Waits for the next frame in the jitter buffer to become complete
  // (waits no longer than maxWaitTimeMs), then passes it to the decoder for
  // decoding.
  // Should be called as often as possible to get the most out of the decoder.
  //
  // Return value      : VCM_OK, on success.
  //                     < 0,    on error.
  virtual int32_t Decode(uint16_t maxWaitTimeMs = 200) = 0;

  // Insert a parsed packet into the receiver side of the module. Will be placed
  // in the
  // jitter buffer waiting for the frame to become complete. Returns as soon as
  // the packet
  // has been placed in the jitter buffer.
  //
  // Input:
  //      - incomingPayload      : Payload of the packet.
  //      - payloadLength        : Length of the payload.
  //      - rtpInfo              : The parsed header.
  //
  // Return value      : VCM_OK, on success.
  //                     < 0,    on error.
  virtual int32_t IncomingPacket(const uint8_t* incomingPayload,
                                 size_t payloadLength,
                                 const WebRtcRTPHeader& rtpInfo) = 0;

  // Robustness APIs

  // DEPRECATED.
  // Set the receiver robustness mode. The mode decides how the receiver
  // responds to losses in the stream. The type of counter-measure is selected
  // through the robustnessMode parameter. The errorMode parameter decides if it
  // is allowed to display frames corrupted by losses. Note that not all
  // combinations of the two parameters are feasible. An error will be
  // returned for invalid combinations.
  // Input:
  //      - robustnessMode : selected robustness mode.
  //      - errorMode      : selected error mode.
  //
  // Return value      : VCM_OK, on success;
  //                     < 0, on error.
  enum ReceiverRobustness { kNone, kHardNack };
  virtual int SetReceiverRobustnessMode(ReceiverRobustness robustnessMode) = 0;

  // Sets the maximum number of sequence numbers that we are allowed to NACK
  // and the oldest sequence number that we will consider to NACK. If a
  // sequence number older than |max_packet_age_to_nack| is missing
  // a key frame will be requested. A key frame will also be requested if the
  // time of incomplete or non-continuous frames in the jitter buffer is above
  // |max_incomplete_time_ms|.
  virtual void SetNackSettings(size_t max_nack_list_size,
                               int max_packet_age_to_nack,
                               int max_incomplete_time_ms) = 0;
};

}  // namespace webrtc

#endif  // MODULES_VIDEO_CODING_INCLUDE_VIDEO_CODING_H_
