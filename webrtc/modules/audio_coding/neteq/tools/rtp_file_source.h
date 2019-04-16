/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_CODING_NETEQ_TOOLS_RTP_FILE_SOURCE_H_
#define MODULES_AUDIO_CODING_NETEQ_TOOLS_RTP_FILE_SOURCE_H_

#include <stdio.h>

#include <memory>
#include <string>

#include "absl/types/optional.h"
#include "modules/audio_coding/neteq/tools/packet_source.h"
#include "modules/rtp_rtcp/include/rtp_rtcp_defines.h"
#include "rtc_base/constructor_magic.h"

namespace webrtc {

class RtpHeaderParser;

namespace test {

class RtpFileReader;

class RtpFileSource : public PacketSource {
 public:
  // Creates an RtpFileSource reading from |file_name|. If the file cannot be
  // opened, or has the wrong format, NULL will be returned.
  static RtpFileSource* Create(
      const std::string& file_name,
      absl::optional<uint32_t> ssrc_filter = absl::nullopt);

  // Checks whether a files is a valid RTP dump or PCAP (Wireshark) file.
  static bool ValidRtpDump(const std::string& file_name);
  static bool ValidPcap(const std::string& file_name);

  ~RtpFileSource() override;

  // Registers an RTP header extension and binds it to |id|.
  virtual bool RegisterRtpHeaderExtension(RTPExtensionType type, uint8_t id);

  std::unique_ptr<Packet> NextPacket() override;

 private:
  static const int kFirstLineLength = 40;
  static const int kRtpFileHeaderSize = 4 + 4 + 4 + 2 + 2;
  static const size_t kPacketHeaderSize = 8;

  explicit RtpFileSource(absl::optional<uint32_t> ssrc_filter);

  bool OpenFile(const std::string& file_name);

  std::unique_ptr<RtpFileReader> rtp_reader_;
  std::unique_ptr<RtpHeaderParser> parser_;
  const absl::optional<uint32_t> ssrc_filter_;

  RTC_DISALLOW_COPY_AND_ASSIGN(RtpFileSource);
};

}  // namespace test
}  // namespace webrtc
#endif  // MODULES_AUDIO_CODING_NETEQ_TOOLS_RTP_FILE_SOURCE_H_
