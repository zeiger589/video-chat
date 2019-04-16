/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_RTP_RTCP_MOCKS_MOCK_RTP_RTCP_H_
#define MODULES_RTP_RTCP_MOCKS_MOCK_RTP_RTCP_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "absl/types/optional.h"
#include "api/video/video_bitrate_allocation.h"
#include "modules/include/module.h"
#include "modules/rtp_rtcp/include/rtp_rtcp.h"
#include "modules/rtp_rtcp/include/rtp_rtcp_defines.h"
#include "modules/rtp_rtcp/source/rtcp_packet/transport_feedback.h"
#include "rtc_base/checks.h"
#include "test/gmock.h"

namespace webrtc {

class MockRtpRtcp : public RtpRtcp {
 public:
  MockRtpRtcp();
  ~MockRtpRtcp();

  MOCK_METHOD2(IncomingRtcpPacket,
               void(const uint8_t* incoming_packet, size_t packet_length));
  MOCK_METHOD1(SetRemoteSSRC, void(uint32_t ssrc));
  MOCK_METHOD1(SetMaxRtpPacketSize, void(size_t size));
  MOCK_CONST_METHOD0(MaxRtpPacketSize, size_t());
  MOCK_METHOD2(RegisterSendPayloadFrequency,
               void(int payload_type, int frequency));
  MOCK_METHOD1(DeRegisterSendPayload, int32_t(int8_t payload_type));
  MOCK_METHOD1(SetExtmapAllowMixed, void(bool extmap_allow_mixed));
  MOCK_METHOD2(RegisterSendRtpHeaderExtension,
               int32_t(RTPExtensionType type, uint8_t id));
  MOCK_METHOD2(RegisterRtpHeaderExtension,
               bool(const std::string& uri, int id));
  MOCK_METHOD1(DeregisterSendRtpHeaderExtension,
               int32_t(RTPExtensionType type));
  MOCK_CONST_METHOD0(HasBweExtensions, bool());
  MOCK_CONST_METHOD0(StartTimestamp, uint32_t());
  MOCK_METHOD1(SetStartTimestamp, void(uint32_t timestamp));
  MOCK_CONST_METHOD0(SequenceNumber, uint16_t());
  MOCK_METHOD1(SetSequenceNumber, void(uint16_t seq));
  MOCK_METHOD1(SetRtpState, void(const RtpState& rtp_state));
  MOCK_METHOD1(SetRtxState, void(const RtpState& rtp_state));
  MOCK_CONST_METHOD0(GetRtpState, RtpState());
  MOCK_CONST_METHOD0(GetRtxState, RtpState());
  MOCK_CONST_METHOD0(SSRC, uint32_t());
  MOCK_METHOD1(SetSSRC, void(uint32_t ssrc));
  MOCK_METHOD1(SetRid, void(const std::string& rid));
  MOCK_METHOD1(SetMid, void(const std::string& mid));
  MOCK_CONST_METHOD1(CSRCs, int32_t(uint32_t csrcs[kRtpCsrcSize]));
  MOCK_METHOD1(SetCsrcs, void(const std::vector<uint32_t>& csrcs));
  MOCK_METHOD1(SetCSRCStatus, int32_t(bool include));
  MOCK_METHOD1(SetRtxSendStatus, void(int modes));
  MOCK_CONST_METHOD0(RtxSendStatus, int());
  MOCK_METHOD1(SetRtxSsrc, void(uint32_t));
  MOCK_METHOD2(SetRtxSendPayloadType, void(int, int));
  MOCK_CONST_METHOD0(FlexfecSsrc, absl::optional<uint32_t>());
  MOCK_CONST_METHOD0(RtxSendPayloadType, std::pair<int, int>());
  MOCK_METHOD1(SetSendingStatus, int32_t(bool sending));
  MOCK_CONST_METHOD0(Sending, bool());
  MOCK_METHOD1(SetSendingMediaStatus, void(bool sending));
  MOCK_CONST_METHOD0(SendingMedia, bool());
  MOCK_METHOD1(SetAsPartOfAllocation, void(bool));
  MOCK_CONST_METHOD4(BitrateSent,
                     void(uint32_t* total_rate,
                          uint32_t* video_rate,
                          uint32_t* fec_rate,
                          uint32_t* nack_rate));
  MOCK_CONST_METHOD1(EstimatedReceiveBandwidth,
                     int(uint32_t* available_bandwidth));
  MOCK_METHOD4(OnSendingRtpFrame, bool(uint32_t, int64_t, int, bool));
  MOCK_METHOD5(TimeToSendPacket,
               bool(uint32_t ssrc,
                    uint16_t sequence_number,
                    int64_t capture_time_ms,
                    bool retransmission,
                    const PacedPacketInfo& pacing_info));
  MOCK_METHOD2(TimeToSendPadding,
               size_t(size_t bytes, const PacedPacketInfo& pacing_info));
  MOCK_METHOD2(RegisterRtcpObservers,
               void(RtcpIntraFrameObserver* intra_frame_callback,
                    RtcpBandwidthObserver* bandwidth_callback));
  MOCK_CONST_METHOD0(RTCP, RtcpMode());
  MOCK_METHOD1(SetRTCPStatus, void(RtcpMode method));
  MOCK_METHOD1(SetCNAME, int32_t(const char cname[RTCP_CNAME_SIZE]));
  MOCK_CONST_METHOD2(RemoteCNAME,
                     int32_t(uint32_t remote_ssrc,
                             char cname[RTCP_CNAME_SIZE]));
  MOCK_CONST_METHOD5(RemoteNTP,
                     int32_t(uint32_t* received_ntp_secs,
                             uint32_t* received_ntp_frac,
                             uint32_t* rtcp_arrival_time_secs,
                             uint32_t* rtcp_arrival_time_frac,
                             uint32_t* rtcp_timestamp));
  MOCK_METHOD2(AddMixedCNAME,
               int32_t(uint32_t ssrc, const char cname[RTCP_CNAME_SIZE]));
  MOCK_METHOD1(RemoveMixedCNAME, int32_t(uint32_t ssrc));
  MOCK_CONST_METHOD5(RTT,
                     int32_t(uint32_t remote_ssrc,
                             int64_t* rtt,
                             int64_t* avg_rtt,
                             int64_t* min_rtt,
                             int64_t* max_rtt));
  MOCK_CONST_METHOD0(ExpectedRetransmissionTimeMs, int64_t());
  MOCK_METHOD1(SendRTCP, int32_t(RTCPPacketType packet_type));
  MOCK_METHOD1(SendCompoundRTCP,
               int32_t(const std::set<RTCPPacketType>& packet_types));
  MOCK_CONST_METHOD2(DataCountersRTP,
                     int32_t(size_t* bytes_sent, uint32_t* packets_sent));
  MOCK_CONST_METHOD2(GetSendStreamDataCounters,
                     void(StreamDataCounters*, StreamDataCounters*));
  MOCK_CONST_METHOD3(GetRtpPacketLossStats,
                     void(bool, uint32_t, struct RtpPacketLossStats*));
  MOCK_CONST_METHOD1(RemoteRTCPStat,
                     int32_t(std::vector<RTCPReportBlock>* receive_blocks));
  MOCK_METHOD4(SetRTCPApplicationSpecificData,
               int32_t(uint8_t sub_type,
                       uint32_t name,
                       const uint8_t* data,
                       uint16_t length));
  MOCK_METHOD1(SetRtcpXrRrtrStatus, void(bool enable));
  MOCK_CONST_METHOD0(RtcpXrRrtrStatus, bool());
  MOCK_METHOD2(SetRemb, void(int64_t bitrate, std::vector<uint32_t> ssrcs));
  MOCK_METHOD0(UnsetRemb, void());
  MOCK_CONST_METHOD0(TMMBR, bool());
  MOCK_METHOD1(SetTMMBRStatus, void(bool enable));
  MOCK_METHOD1(OnBandwidthEstimateUpdate, void(uint16_t bandwidth_kbit));
  MOCK_METHOD2(SendNACK, int32_t(const uint16_t* nack_list, uint16_t size));
  MOCK_METHOD1(SendNack, void(const std::vector<uint16_t>& sequence_numbers));
  MOCK_METHOD2(SetStorePacketsStatus,
               void(bool enable, uint16_t number_to_store));
  MOCK_CONST_METHOD0(StorePackets, bool());
  MOCK_METHOD1(RegisterRtcpStatisticsCallback, void(RtcpStatisticsCallback*));
  MOCK_METHOD0(GetRtcpStatisticsCallback, RtcpStatisticsCallback*());
  MOCK_METHOD1(SendFeedbackPacket, bool(const rtcp::TransportFeedback& packet));
  MOCK_METHOD1(SetTargetSendBitrate, void(uint32_t bitrate_bps));
  MOCK_METHOD1(SetKeyFrameRequestMethod, int32_t(KeyFrameRequestMethod method));
  MOCK_METHOD0(RequestKeyFrame, int32_t());
  MOCK_METHOD3(SendLossNotification,
               int32_t(uint16_t last_decoded_seq_num,
                       uint16_t last_received_seq_num,
                       bool decodability_flag));
  MOCK_METHOD0(Process, void());
  MOCK_METHOD1(RegisterSendChannelRtpStatisticsCallback,
               void(StreamDataCountersCallback*));
  MOCK_CONST_METHOD0(GetSendChannelRtpStatisticsCallback,
                     StreamDataCountersCallback*(void));
  MOCK_METHOD1(SetVideoBitrateAllocation, void(const VideoBitrateAllocation&));
  MOCK_METHOD0(RtpSender, RTPSender*());
  MOCK_CONST_METHOD0(RtpSender, const RTPSender*());

  // Members.
  unsigned int remote_ssrc_;

 private:
  // Mocking this method is currently not required and having a default
  // implementation like MOCK_METHOD0(TimeUntilNextProcess, int64_t())
  // can be dangerous since it can cause a tight loop on a process thread.
  virtual int64_t TimeUntilNextProcess() { return 0xffffffff; }
};

}  // namespace webrtc

#endif  // MODULES_RTP_RTCP_MOCKS_MOCK_RTP_RTCP_H_
