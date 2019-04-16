// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "absl/numeric/int128.h"

#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace {

struct Uint128TestCase {
  absl::uint128 value;
  std::ios_base::fmtflags flags;
  std::streamsize width;
  const char* expected;
};

constexpr char kFill = '_';

std::string StreamFormatToString(std::ios_base::fmtflags flags,
                                 std::streamsize width) {
  std::vector<const char*> flagstr;
  switch (flags & std::ios::basefield) {
    case std::ios::dec:
      flagstr.push_back("std::ios::dec");
      break;
    case std::ios::oct:
      flagstr.push_back("std::ios::oct");
      break;
    case std::ios::hex:
      flagstr.push_back("std::ios::hex");
      break;
    default:  // basefield not specified
      break;
  }
  switch (flags & std::ios::adjustfield) {
    case std::ios::left:
      flagstr.push_back("std::ios::left");
      break;
    case std::ios::internal:
      flagstr.push_back("std::ios::internal");
      break;
    case std::ios::right:
      flagstr.push_back("std::ios::right");
      break;
    default:  // adjustfield not specified
      break;
  }
  if (flags & std::ios::uppercase) flagstr.push_back("std::ios::uppercase");
  if (flags & std::ios::showbase) flagstr.push_back("std::ios::showbase");
  if (flags & std::ios::showpos) flagstr.push_back("std::ios::showpos");

  std::ostringstream msg;
  msg << "\n  StreamFormatToString(test_case.flags, test_case.width)\n    "
         "flags: ";
  if (!flagstr.empty()) {
    for (size_t i = 0; i < flagstr.size() - 1; ++i) msg << flagstr[i] << " | ";
    msg << flagstr.back();
  } else {
    msg << "(default)";
  }
  msg << "\n    width: " << width << "\n    fill: '" << kFill << "'";
  return msg.str();
}

void CheckUint128Case(const Uint128TestCase& test_case) {
  std::ostringstream os;
  os.flags(test_case.flags);
  os.width(test_case.width);
  os.fill(kFill);
  os << test_case.value;
  SCOPED_TRACE(StreamFormatToString(test_case.flags, test_case.width));
  EXPECT_EQ(test_case.expected, os.str());
}

constexpr std::ios::fmtflags kDec = std::ios::dec;
constexpr std::ios::fmtflags kOct = std::ios::oct;
constexpr std::ios::fmtflags kHex = std::ios::hex;
constexpr std::ios::fmtflags kLeft = std::ios::left;
constexpr std::ios::fmtflags kInt = std::ios::internal;
constexpr std::ios::fmtflags kRight = std::ios::right;
constexpr std::ios::fmtflags kUpper = std::ios::uppercase;
constexpr std::ios::fmtflags kBase = std::ios::showbase;
constexpr std::ios::fmtflags kPos = std::ios::showpos;

TEST(Uint128, OStreamValueTest) {
  CheckUint128Case({1, kDec, /*width = */ 0, "1"});
  CheckUint128Case({1, kOct, /*width = */ 0, "1"});
  CheckUint128Case({1, kHex, /*width = */ 0, "1"});
  CheckUint128Case({9, kDec, /*width = */ 0, "9"});
  CheckUint128Case({9, kOct, /*width = */ 0, "11"});
  CheckUint128Case({9, kHex, /*width = */ 0, "9"});
  CheckUint128Case({12345, kDec, /*width = */ 0, "12345"});
  CheckUint128Case({12345, kOct, /*width = */ 0, "30071"});
  CheckUint128Case({12345, kHex, /*width = */ 0, "3039"});
  CheckUint128Case(
      {0x8000000000000000, kDec, /*width = */ 0, "9223372036854775808"});
  CheckUint128Case(
      {0x8000000000000000, kOct, /*width = */ 0, "1000000000000000000000"});
  CheckUint128Case(
      {0x8000000000000000, kHex, /*width = */ 0, "8000000000000000"});
  CheckUint128Case({std::numeric_limits<uint64_t>::max(), kDec,
                    /*width = */ 0, "18446744073709551615"});
  CheckUint128Case({std::numeric_limits<uint64_t>::max(), kOct,
                    /*width = */ 0, "1777777777777777777777"});
  CheckUint128Case({std::numeric_limits<uint64_t>::max(), kHex,
                    /*width = */ 0, "ffffffffffffffff"});
  CheckUint128Case(
      {absl::MakeUint128(1, 0), kDec, /*width = */ 0, "18446744073709551616"});
  CheckUint128Case({absl::MakeUint128(1, 0), kOct, /*width = */ 0,
                    "2000000000000000000000"});
  CheckUint128Case(
      {absl::MakeUint128(1, 0), kHex, /*width = */ 0, "10000000000000000"});
  CheckUint128Case({absl::MakeUint128(0x8000000000000000, 0), kDec,
                    /*width = */ 0, "170141183460469231731687303715884105728"});
  CheckUint128Case({absl::MakeUint128(0x8000000000000000, 0), kOct,
                    /*width = */ 0,
                    "2000000000000000000000000000000000000000000"});
  CheckUint128Case({absl::MakeUint128(0x8000000000000000, 0), kHex,
                    /*width = */ 0, "80000000000000000000000000000000"});
  CheckUint128Case({absl::kuint128max, kDec, /*width = */ 0,
                    "340282366920938463463374607431768211455"});
  CheckUint128Case({absl::kuint128max, kOct, /*width = */ 0,
                    "3777777777777777777777777777777777777777777"});
  CheckUint128Case({absl::kuint128max, kHex, /*width = */ 0,
                    "ffffffffffffffffffffffffffffffff"});
}

std::vector<Uint128TestCase> GetUint128FormatCases();

TEST(Uint128, OStreamFormatTest) {
  for (const Uint128TestCase& test_case : GetUint128FormatCases()) {
    CheckUint128Case(test_case);
  }
}

std::vector<Uint128TestCase> GetUint128FormatCases() {
  return {
      {0, std::ios_base::fmtflags(), /*width = */ 0, "0"},
      {0, std::ios_base::fmtflags(), /*width = */ 6, "_____0"},
      {0, kPos, /*width = */ 0, "0"},
      {0, kPos, /*width = */ 6, "_____0"},
      {0, kBase, /*width = */ 0, "0"},
      {0, kBase, /*width = */ 6, "_____0"},
      {0, kBase | kPos, /*width = */ 0, "0"},
      {0, kBase | kPos, /*width = */ 6, "_____0"},
      {0, kUpper, /*width = */ 0, "0"},
      {0, kUpper, /*width = */ 6, "_____0"},
      {0, kUpper | kPos, /*width = */ 0, "0"},
      {0, kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kUpper | kBase, /*width = */ 0, "0"},
      {0, kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kLeft, /*width = */ 0, "0"},
      {0, kLeft, /*width = */ 6, "0_____"},
      {0, kLeft | kPos, /*width = */ 0, "0"},
      {0, kLeft | kPos, /*width = */ 6, "0_____"},
      {0, kLeft | kBase, /*width = */ 0, "0"},
      {0, kLeft | kBase, /*width = */ 6, "0_____"},
      {0, kLeft | kBase | kPos, /*width = */ 0, "0"},
      {0, kLeft | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kLeft | kUpper, /*width = */ 0, "0"},
      {0, kLeft | kUpper, /*width = */ 6, "0_____"},
      {0, kLeft | kUpper | kPos, /*width = */ 0, "0"},
      {0, kLeft | kUpper | kPos, /*width = */ 6, "0_____"},
      {0, kLeft | kUpper | kBase, /*width = */ 0, "0"},
      {0, kLeft | kUpper | kBase, /*width = */ 6, "0_____"},
      {0, kLeft | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kLeft | kUpper | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kInt, /*width = */ 0, "0"},
      {0, kInt, /*width = */ 6, "_____0"},
      {0, kInt | kPos, /*width = */ 0, "0"},
      {0, kInt | kPos, /*width = */ 6, "_____0"},
      {0, kInt | kBase, /*width = */ 0, "0"},
      {0, kInt | kBase, /*width = */ 6, "_____0"},
      {0, kInt | kBase | kPos, /*width = */ 0, "0"},
      {0, kInt | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kInt | kUpper, /*width = */ 0, "0"},
      {0, kInt | kUpper, /*width = */ 6, "_____0"},
      {0, kInt | kUpper | kPos, /*width = */ 0, "0"},
      {0, kInt | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kInt | kUpper | kBase, /*width = */ 0, "0"},
      {0, kInt | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kInt | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kInt | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kRight, /*width = */ 0, "0"},
      {0, kRight, /*width = */ 6, "_____0"},
      {0, kRight | kPos, /*width = */ 0, "0"},
      {0, kRight | kPos, /*width = */ 6, "_____0"},
      {0, kRight | kBase, /*width = */ 0, "0"},
      {0, kRight | kBase, /*width = */ 6, "_____0"},
      {0, kRight | kBase | kPos, /*width = */ 0, "0"},
      {0, kRight | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kRight | kUpper, /*width = */ 0, "0"},
      {0, kRight | kUpper, /*width = */ 6, "_____0"},
      {0, kRight | kUpper | kPos, /*width = */ 0, "0"},
      {0, kRight | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kRight | kUpper | kBase, /*width = */ 0, "0"},
      {0, kRight | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kRight | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kRight | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec, /*width = */ 0, "0"},
      {0, kDec, /*width = */ 6, "_____0"},
      {0, kDec | kPos, /*width = */ 0, "0"},
      {0, kDec | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kBase, /*width = */ 0, "0"},
      {0, kDec | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kUpper, /*width = */ 0, "0"},
      {0, kDec | kUpper, /*width = */ 6, "_____0"},
      {0, kDec | kUpper | kPos, /*width = */ 0, "0"},
      {0, kDec | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kUpper | kBase, /*width = */ 0, "0"},
      {0, kDec | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kLeft, /*width = */ 0, "0"},
      {0, kDec | kLeft, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kPos, /*width = */ 0, "0"},
      {0, kDec | kLeft | kPos, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kBase, /*width = */ 0, "0"},
      {0, kDec | kLeft | kBase, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kLeft | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kUpper, /*width = */ 0, "0"},
      {0, kDec | kLeft | kUpper, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kUpper | kPos, /*width = */ 0, "0"},
      {0, kDec | kLeft | kUpper | kPos, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kUpper | kBase, /*width = */ 0, "0"},
      {0, kDec | kLeft | kUpper | kBase, /*width = */ 6, "0_____"},
      {0, kDec | kLeft | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kLeft | kUpper | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kDec | kInt, /*width = */ 0, "0"},
      {0, kDec | kInt, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kPos, /*width = */ 0, "0"},
      {0, kDec | kInt | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kBase, /*width = */ 0, "0"},
      {0, kDec | kInt | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kInt | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kUpper, /*width = */ 0, "0"},
      {0, kDec | kInt | kUpper, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kUpper | kPos, /*width = */ 0, "0"},
      {0, kDec | kInt | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kUpper | kBase, /*width = */ 0, "0"},
      {0, kDec | kInt | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kInt | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kInt | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kRight, /*width = */ 0, "0"},
      {0, kDec | kRight, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kPos, /*width = */ 0, "0"},
      {0, kDec | kRight | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kBase, /*width = */ 0, "0"},
      {0, kDec | kRight | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kRight | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kUpper, /*width = */ 0, "0"},
      {0, kDec | kRight | kUpper, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kUpper | kPos, /*width = */ 0, "0"},
      {0, kDec | kRight | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kUpper | kBase, /*width = */ 0, "0"},
      {0, kDec | kRight | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kDec | kRight | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kDec | kRight | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct, /*width = */ 0, "0"},
      {0, kOct, /*width = */ 6, "_____0"},
      {0, kOct | kPos, /*width = */ 0, "0"},
      {0, kOct | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kBase, /*width = */ 0, "0"},
      {0, kOct | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kUpper, /*width = */ 0, "0"},
      {0, kOct | kUpper, /*width = */ 6, "_____0"},
      {0, kOct | kUpper | kPos, /*width = */ 0, "0"},
      {0, kOct | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kUpper | kBase, /*width = */ 0, "0"},
      {0, kOct | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kLeft, /*width = */ 0, "0"},
      {0, kOct | kLeft, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kPos, /*width = */ 0, "0"},
      {0, kOct | kLeft | kPos, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kBase, /*width = */ 0, "0"},
      {0, kOct | kLeft | kBase, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kLeft | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kUpper, /*width = */ 0, "0"},
      {0, kOct | kLeft | kUpper, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kUpper | kPos, /*width = */ 0, "0"},
      {0, kOct | kLeft | kUpper | kPos, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kUpper | kBase, /*width = */ 0, "0"},
      {0, kOct | kLeft | kUpper | kBase, /*width = */ 6, "0_____"},
      {0, kOct | kLeft | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kLeft | kUpper | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kOct | kInt, /*width = */ 0, "0"},
      {0, kOct | kInt, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kPos, /*width = */ 0, "0"},
      {0, kOct | kInt | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kBase, /*width = */ 0, "0"},
      {0, kOct | kInt | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kInt | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kUpper, /*width = */ 0, "0"},
      {0, kOct | kInt | kUpper, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kUpper | kPos, /*width = */ 0, "0"},
      {0, kOct | kInt | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kUpper | kBase, /*width = */ 0, "0"},
      {0, kOct | kInt | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kInt | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kInt | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kRight, /*width = */ 0, "0"},
      {0, kOct | kRight, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kPos, /*width = */ 0, "0"},
      {0, kOct | kRight | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kBase, /*width = */ 0, "0"},
      {0, kOct | kRight | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kRight | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kUpper, /*width = */ 0, "0"},
      {0, kOct | kRight | kUpper, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kUpper | kPos, /*width = */ 0, "0"},
      {0, kOct | kRight | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kUpper | kBase, /*width = */ 0, "0"},
      {0, kOct | kRight | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kOct | kRight | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kOct | kRight | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex, /*width = */ 0, "0"},
      {0, kHex, /*width = */ 6, "_____0"},
      {0, kHex | kPos, /*width = */ 0, "0"},
      {0, kHex | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kBase, /*width = */ 0, "0"},
      {0, kHex | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kUpper, /*width = */ 0, "0"},
      {0, kHex | kUpper, /*width = */ 6, "_____0"},
      {0, kHex | kUpper | kPos, /*width = */ 0, "0"},
      {0, kHex | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kUpper | kBase, /*width = */ 0, "0"},
      {0, kHex | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kLeft, /*width = */ 0, "0"},
      {0, kHex | kLeft, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kPos, /*width = */ 0, "0"},
      {0, kHex | kLeft | kPos, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kBase, /*width = */ 0, "0"},
      {0, kHex | kLeft | kBase, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kLeft | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kUpper, /*width = */ 0, "0"},
      {0, kHex | kLeft | kUpper, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kUpper | kPos, /*width = */ 0, "0"},
      {0, kHex | kLeft | kUpper | kPos, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kUpper | kBase, /*width = */ 0, "0"},
      {0, kHex | kLeft | kUpper | kBase, /*width = */ 6, "0_____"},
      {0, kHex | kLeft | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kLeft | kUpper | kBase | kPos, /*width = */ 6, "0_____"},
      {0, kHex | kInt, /*width = */ 0, "0"},
      {0, kHex | kInt, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kPos, /*width = */ 0, "0"},
      {0, kHex | kInt | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kBase, /*width = */ 0, "0"},
      {0, kHex | kInt | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kInt | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kUpper, /*width = */ 0, "0"},
      {0, kHex | kInt | kUpper, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kUpper | kPos, /*width = */ 0, "0"},
      {0, kHex | kInt | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kUpper | kBase, /*width = */ 0, "0"},
      {0, kHex | kInt | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kInt | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kInt | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kRight, /*width = */ 0, "0"},
      {0, kHex | kRight, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kPos, /*width = */ 0, "0"},
      {0, kHex | kRight | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kBase, /*width = */ 0, "0"},
      {0, kHex | kRight | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kRight | kBase | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kUpper, /*width = */ 0, "0"},
      {0, kHex | kRight | kUpper, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kUpper | kPos, /*width = */ 0, "0"},
      {0, kHex | kRight | kUpper | kPos, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kUpper | kBase, /*width = */ 0, "0"},
      {0, kHex | kRight | kUpper | kBase, /*width = */ 6, "_____0"},
      {0, kHex | kRight | kUpper | kBase | kPos, /*width = */ 0, "0"},
      {0, kHex | kRight | kUpper | kBase | kPos, /*width = */ 6, "_____0"},
      {37, std::ios_base::fmtflags(), /*width = */ 0, "37"},
      {37, std::ios_base::fmtflags(), /*width = */ 6, "____37"},
      {37, kPos, /*width = */ 0, "37"},
      {37, kPos, /*width = */ 6, "____37"},
      {37, kBase, /*width = */ 0, "37"},
      {37, kBase, /*width = */ 6, "____37"},
      {37, kBase | kPos, /*width = */ 0, "37"},
      {37, kBase | kPos, /*width = */ 6, "____37"},
      {37, kUpper, /*width = */ 0, "37"},
      {37, kUpper, /*width = */ 6, "____37"},
      {37, kUpper | kPos, /*width = */ 0, "37"},
      {37, kUpper | kPos, /*width = */ 6, "____37"},
      {37, kUpper | kBase, /*width = */ 0, "37"},
      {37, kUpper | kBase, /*width = */ 6, "____37"},
      {37, kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kLeft, /*width = */ 0, "37"},
      {37, kLeft, /*width = */ 6, "37____"},
      {37, kLeft | kPos, /*width = */ 0, "37"},
      {37, kLeft | kPos, /*width = */ 6, "37____"},
      {37, kLeft | kBase, /*width = */ 0, "37"},
      {37, kLeft | kBase, /*width = */ 6, "37____"},
      {37, kLeft | kBase | kPos, /*width = */ 0, "37"},
      {37, kLeft | kBase | kPos, /*width = */ 6, "37____"},
      {37, kLeft | kUpper, /*width = */ 0, "37"},
      {37, kLeft | kUpper, /*width = */ 6, "37____"},
      {37, kLeft | kUpper | kPos, /*width = */ 0, "37"},
      {37, kLeft | kUpper | kPos, /*width = */ 6, "37____"},
      {37, kLeft | kUpper | kBase, /*width = */ 0, "37"},
      {37, kLeft | kUpper | kBase, /*width = */ 6, "37____"},
      {37, kLeft | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kLeft | kUpper | kBase | kPos, /*width = */ 6, "37____"},
      {37, kInt, /*width = */ 0, "37"},
      {37, kInt, /*width = */ 6, "____37"},
      {37, kInt | kPos, /*width = */ 0, "37"},
      {37, kInt | kPos, /*width = */ 6, "____37"},
      {37, kInt | kBase, /*width = */ 0, "37"},
      {37, kInt | kBase, /*width = */ 6, "____37"},
      {37, kInt | kBase | kPos, /*width = */ 0, "37"},
      {37, kInt | kBase | kPos, /*width = */ 6, "____37"},
      {37, kInt | kUpper, /*width = */ 0, "37"},
      {37, kInt | kUpper, /*width = */ 6, "____37"},
      {37, kInt | kUpper | kPos, /*width = */ 0, "37"},
      {37, kInt | kUpper | kPos, /*width = */ 6, "____37"},
      {37, kInt | kUpper | kBase, /*width = */ 0, "37"},
      {37, kInt | kUpper | kBase, /*width = */ 6, "____37"},
      {37, kInt | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kInt | kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kRight, /*width = */ 0, "37"},
      {37, kRight, /*width = */ 6, "____37"},
      {37, kRight | kPos, /*width = */ 0, "37"},
      {37, kRight | kPos, /*width = */ 6, "____37"},
      {37, kRight | kBase, /*width = */ 0, "37"},
      {37, kRight | kBase, /*width = */ 6, "____37"},
      {37, kRight | kBase | kPos, /*width = */ 0, "37"},
      {37, kRight | kBase | kPos, /*width = */ 6, "____37"},
      {37, kRight | kUpper, /*width = */ 0, "37"},
      {37, kRight | kUpper, /*width = */ 6, "____37"},
      {37, kRight | kUpper | kPos, /*width = */ 0, "37"},
      {37, kRight | kUpper | kPos, /*width = */ 6, "____37"},
      {37, kRight | kUpper | kBase, /*width = */ 0, "37"},
      {37, kRight | kUpper | kBase, /*width = */ 6, "____37"},
      {37, kRight | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kRight | kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec, /*width = */ 0, "37"},
      {37, kDec, /*width = */ 6, "____37"},
      {37, kDec | kPos, /*width = */ 0, "37"},
      {37, kDec | kPos, /*width = */ 6, "____37"},
      {37, kDec | kBase, /*width = */ 0, "37"},
      {37, kDec | kBase, /*width = */ 6, "____37"},
      {37, kDec | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec | kUpper, /*width = */ 0, "37"},
      {37, kDec | kUpper, /*width = */ 6, "____37"},
      {37, kDec | kUpper | kPos, /*width = */ 0, "37"},
      {37, kDec | kUpper | kPos, /*width = */ 6, "____37"},
      {37, kDec | kUpper | kBase, /*width = */ 0, "37"},
      {37, kDec | kUpper | kBase, /*width = */ 6, "____37"},
      {37, kDec | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec | kLeft, /*width = */ 0, "37"},
      {37, kDec | kLeft, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kPos, /*width = */ 0, "37"},
      {37, kDec | kLeft | kPos, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kBase, /*width = */ 0, "37"},
      {37, kDec | kLeft | kBase, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kLeft | kBase | kPos, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kUpper, /*width = */ 0, "37"},
      {37, kDec | kLeft | kUpper, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kUpper | kPos, /*width = */ 0, "37"},
      {37, kDec | kLeft | kUpper | kPos, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kUpper | kBase, /*width = */ 0, "37"},
      {37, kDec | kLeft | kUpper | kBase, /*width = */ 6, "37____"},
      {37, kDec | kLeft | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kLeft | kUpper | kBase | kPos, /*width = */ 6, "37____"},
      {37, kDec | kInt, /*width = */ 0, "37"},
      {37, kDec | kInt, /*width = */ 6, "____37"},
      {37, kDec | kInt | kPos, /*width = */ 0, "37"},
      {37, kDec | kInt | kPos, /*width = */ 6, "____37"},
      {37, kDec | kInt | kBase, /*width = */ 0, "37"},
      {37, kDec | kInt | kBase, /*width = */ 6, "____37"},
      {37, kDec | kInt | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kInt | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec | kInt | kUpper, /*width = */ 0, "37"},
      {37, kDec | kInt | kUpper, /*width = */ 6, "____37"},
      {37, kDec | kInt | kUpper | kPos, /*width = */ 0, "37"},
      {37, kDec | kInt | kUpper | kPos, /*width = */ 6, "____37"},
      {37, kDec | kInt | kUpper | kBase, /*width = */ 0, "37"},
      {37, kDec | kInt | kUpper | kBase, /*width = */ 6, "____37"},
      {37, kDec | kInt | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kInt | kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec | kRight, /*width = */ 0, "37"},
      {37, kDec | kRight, /*width = */ 6, "____37"},
      {37, kDec | kRight | kPos, /*width = */ 0, "37"},
      {37, kDec | kRight | kPos, /*width = */ 6, "____37"},
      {37, kDec | kRight | kBase, /*width = */ 0, "37"},
      {37, kDec | kRight | kBase, /*width = */ 6, "____37"},
      {37, kDec | kRight | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kRight | kBase | kPos, /*width = */ 6, "____37"},
      {37, kDec | kRight | kUpper, /*width = */ 0, "37"},
      {37, kDec | kRight | kUpper, /*width = */ 6, "____37"},
      {37, kDec | kRight | kUpper | kPos, /*width = */ 0, "37"},
      {37, kDec | kRight | kUpper | kPos, /*width = */ 6, "____37"},
      {37, kDec | kRight | kUpper | kBase, /*width = */ 0, "37"},
      {37, kDec | kRight | kUpper | kBase, /*width = */ 6, "____37"},
      {37, kDec | kRight | kUpper | kBase | kPos, /*width = */ 0, "37"},
      {37, kDec | kRight | kUpper | kBase | kPos, /*width = */ 6, "____37"},
      {37, kOct, /*width = */ 0, "45"},
      {37, kOct, /*width = */ 6, "____45"},
      {37, kOct | kPos, /*width = */ 0, "45"},
      {37, kOct | kPos, /*width = */ 6, "____45"},
      {37, kOct | kBase, /*width = */ 0, "045"},
      {37, kOct | kBase, /*width = */ 6, "___045"},
      {37, kOct | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kBase | kPos, /*width = */ 6, "___045"},
      {37, kOct | kUpper, /*width = */ 0, "45"},
      {37, kOct | kUpper, /*width = */ 6, "____45"},
      {37, kOct | kUpper | kPos, /*width = */ 0, "45"},
      {37, kOct | kUpper | kPos, /*width = */ 6, "____45"},
      {37, kOct | kUpper | kBase, /*width = */ 0, "045"},
      {37, kOct | kUpper | kBase, /*width = */ 6, "___045"},
      {37, kOct | kUpper | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kUpper | kBase | kPos, /*width = */ 6, "___045"},
      {37, kOct | kLeft, /*width = */ 0, "45"},
      {37, kOct | kLeft, /*width = */ 6, "45____"},
      {37, kOct | kLeft | kPos, /*width = */ 0, "45"},
      {37, kOct | kLeft | kPos, /*width = */ 6, "45____"},
      {37, kOct | kLeft | kBase, /*width = */ 0, "045"},
      {37, kOct | kLeft | kBase, /*width = */ 6, "045___"},
      {37, kOct | kLeft | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kLeft | kBase | kPos, /*width = */ 6, "045___"},
      {37, kOct | kLeft | kUpper, /*width = */ 0, "45"},
      {37, kOct | kLeft | kUpper, /*width = */ 6, "45____"},
      {37, kOct | kLeft | kUpper | kPos, /*width = */ 0, "45"},
      {37, kOct | kLeft | kUpper | kPos, /*width = */ 6, "45____"},
      {37, kOct | kLeft | kUpper | kBase, /*width = */ 0, "045"},
      {37, kOct | kLeft | kUpper | kBase, /*width = */ 6, "045___"},
      {37, kOct | kLeft | kUpper | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kLeft | kUpper | kBase | kPos, /*width = */ 6, "045___"},
      {37, kOct | kInt, /*width = */ 0, "45"},
      {37, kOct | kInt, /*width = */ 6, "____45"},
      {37, kOct | kInt | kPos, /*width = */ 0, "45"},
      {37, kOct | kInt | kPos, /*width = */ 6, "____45"},
      {37, kOct | kInt | kBase, /*width = */ 0, "045"},
      {37, kOct | kInt | kBase, /*width = */ 6, "___045"},
      {37, kOct | kInt | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kInt | kBase | kPos, /*width = */ 6, "___045"},
      {37, kOct | kInt | kUpper, /*width = */ 0, "45"},
      {37, kOct | kInt | kUpper, /*width = */ 6, "____45"},
      {37, kOct | kInt | kUpper | kPos, /*width = */ 0, "45"},
      {37, kOct | kInt | kUpper | kPos, /*width = */ 6, "____45"},
      {37, kOct | kInt | kUpper | kBase, /*width = */ 0, "045"},
      {37, kOct | kInt | kUpper | kBase, /*width = */ 6, "___045"},
      {37, kOct | kInt | kUpper | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kInt | kUpper | kBase | kPos, /*width = */ 6, "___045"},
      {37, kOct | kRight, /*width = */ 0, "45"},
      {37, kOct | kRight, /*width = */ 6, "____45"},
      {37, kOct | kRight | kPos, /*width = */ 0, "45"},
      {37, kOct | kRight | kPos, /*width = */ 6, "____45"},
      {37, kOct | kRight | kBase, /*width = */ 0, "045"},
      {37, kOct | kRight | kBase, /*width = */ 6, "___045"},
      {37, kOct | kRight | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kRight | kBase | kPos, /*width = */ 6, "___045"},
      {37, kOct | kRight | kUpper, /*width = */ 0, "45"},
      {37, kOct | kRight | kUpper, /*width = */ 6, "____45"},
      {37, kOct | kRight | kUpper | kPos, /*width = */ 0, "45"},
      {37, kOct | kRight | kUpper | kPos, /*width = */ 6, "____45"},
      {37, kOct | kRight | kUpper | kBase, /*width = */ 0, "045"},
      {37, kOct | kRight | kUpper | kBase, /*width = */ 6, "___045"},
      {37, kOct | kRight | kUpper | kBase | kPos, /*width = */ 0, "045"},
      {37, kOct | kRight | kUpper | kBase | kPos, /*width = */ 6, "___045"},
      {37, kHex, /*width = */ 0, "25"},
      {37, kHex, /*width = */ 6, "____25"},
      {37, kHex | kPos, /*width = */ 0, "25"},
      {37, kHex | kPos, /*width = */ 6, "____25"},
      {37, kHex | kBase, /*width = */ 0, "0x25"},
      {37, kHex | kBase, /*width = */ 6, "__0x25"},
      {37, kHex | kBase | kPos, /*width = */ 0, "0x25"},
      {37, kHex | kBase | kPos, /*width = */ 6, "__0x25"},
      {37, kHex | kUpper, /*width = */ 0, "25"},
      {37, kHex | kUpper, /*width = */ 6, "____25"},
      {37, kHex | kUpper | kPos, /*width = */ 0, "25"},
      {37, kHex | kUpper | kPos, /*width = */ 6, "____25"},
      {37, kHex | kUpper | kBase, /*width = */ 0, "0X25"},
      {37, kHex | kUpper | kBase, /*width = */ 6, "__0X25"},
      {37, kHex | kUpper | kBase | kPos, /*width = */ 0, "0X25"},
      {37, kHex | kUpper | kBase | kPos, /*width = */ 6, "__0X25"},
      {37, kHex | kLeft, /*width = */ 0, "25"},
      {37, kHex | kLeft, /*width = */ 6, "25____"},
      {37, kHex | kLeft | kPos, /*width = */ 0, "25"},
      {37, kHex | kLeft | kPos, /*width = */ 6, "25____"},
      {37, kHex | kLeft | kBase, /*width = */ 0, "0x25"},
      {37, kHex | kLeft | kBase, /*width = */ 6, "0x25__"},
      {37, kHex | kLeft | kBase | kPos, /*width = */ 0, "0x25"},
      {37, kHex | kLeft | kBase | kPos, /*width = */ 6, "0x25__"},
      {37, kHex | kLeft | kUpper, /*width = */ 0, "25"},
      {37, kHex | kLeft | kUpper, /*width = */ 6, "25____"},
      {37, kHex | kLeft | kUpper | kPos, /*width = */ 0, "25"},
      {37, kHex | kLeft | kUpper | kPos, /*width = */ 6, "25____"},
      {37, kHex | kLeft | kUpper | kBase, /*width = */ 0, "0X25"},
      {37, kHex | kLeft | kUpper | kBase, /*width = */ 6, "0X25__"},
      {37, kHex | kLeft | kUpper | kBase | kPos, /*width = */ 0, "0X25"},
      {37, kHex | kLeft | kUpper | kBase | kPos, /*width = */ 6, "0X25__"},
      {37, kHex | kInt, /*width = */ 0, "25"},
      {37, kHex | kInt, /*width = */ 6, "____25"},
      {37, kHex | kInt | kPos, /*width = */ 0, "25"},
      {37, kHex | kInt | kPos, /*width = */ 6, "____25"},
      {37, kHex | kInt | kBase, /*width = */ 0, "0x25"},
      {37, kHex | kInt | kBase, /*width = */ 6, "0x__25"},
      {37, kHex | kInt | kBase | kPos, /*width = */ 0, "0x25"},
      {37, kHex | kInt | kBase | kPos, /*width = */ 6, "0x__25"},
      {37, kHex | kInt | kUpper, /*width = */ 0, "25"},
      {37, kHex | kInt | kUpper, /*width = */ 6, "____25"},
      {37, kHex | kInt | kUpper | kPos, /*width = */ 0, "25"},
      {37, kHex | kInt | kUpper | kPos, /*width = */ 6, "____25"},
      {37, kHex | kInt | kUpper | kBase, /*width = */ 0, "0X25"},
      {37, kHex | kInt | kUpper | kBase, /*width = */ 6, "0X__25"},
      {37, kHex | kInt | kUpper | kBase | kPos, /*width = */ 0, "0X25"},
      {37, kHex | kInt | kUpper | kBase | kPos, /*width = */ 6, "0X__25"},
      {37, kHex | kRight, /*width = */ 0, "25"},
      {37, kHex | kRight, /*width = */ 6, "____25"},
      {37, kHex | kRight | kPos, /*width = */ 0, "25"},
      {37, kHex | kRight | kPos, /*width = */ 6, "____25"},
      {37, kHex | kRight | kBase, /*width = */ 0, "0x25"},
      {37, kHex | kRight | kBase, /*width = */ 6, "__0x25"},
      {37, kHex | kRight | kBase | kPos, /*width = */ 0, "0x25"},
      {37, kHex | kRight | kBase | kPos, /*width = */ 6, "__0x25"},
      {37, kHex | kRight | kUpper, /*width = */ 0, "25"},
      {37, kHex | kRight | kUpper, /*width = */ 6, "____25"},
      {37, kHex | kRight | kUpper | kPos, /*width = */ 0, "25"},
      {37, kHex | kRight | kUpper | kPos, /*width = */ 6, "____25"},
      {37, kHex | kRight | kUpper | kBase, /*width = */ 0, "0X25"},
      {37, kHex | kRight | kUpper | kBase, /*width = */ 6, "__0X25"},
      {37, kHex | kRight | kUpper | kBase | kPos, /*width = */ 0, "0X25"},
      {37, kHex | kRight | kUpper | kBase | kPos, /*width = */ 6, "__0X25"}};
}

}  // namespace
