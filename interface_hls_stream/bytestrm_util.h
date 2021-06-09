/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BYTESTRM_UTIL_H_
#define BYTESTRM_UTIL_H_

#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace std;

#define MAX_BYTESTRM_LEN 1488

// Convert a stream of bytes (uint8_t) to a stream of arbitrary width words
// (operates on byte boundaries)
template<typename T, uint8_t NB, bool BS>
void strm_bytes2words(
      hls::stream<T> &words_out,
      hls::stream<uint8_t> &bytes_in,
      uint16_t bytestrm_len
      )
{
   ap_uint<NB * 8> tmpword;
   uint8_t tmpbyte;
BYTES2WORDS_LOOP:
   for (int i = 0; i < (int)bytestrm_len; i++) {
#pragma HLS loop_tripcount max=1488
      bytes_in.read(tmpbyte); // or: bytes_in >> tmpbyte; or inval = bytes_in.read();
      if (!BS) { // Shift bytes in little endian order
         tmpword = (tmpword >> 8) |
            (ap_uint<NB * 8>(tmpbyte) << ((NB - 1) * 8));
      } else { // Shift bytes in big endian order
         tmpword = (tmpword << 8) | ap_uint<NB * 8>(tmpbyte);
      }
      if (i % NB == NB - 1)
         words_out.write(T(tmpword)); // or: words_out << T(tmpword);
   }
}

// Convert an arbitrary width (on byte boundaries) words into a stream of
// bytes (uint8_t)
template<typename T, uint8_t NB, bool BS>
void strm_words2bytes(
      hls::stream<uint8_t> &bytes_out,
      hls::stream<T> &words_in,
      uint16_t bytestrm_len
      )
{
   T inval;
   ap_uint<NB * 8> tmpword;
   uint8_t tmpbyte;
WORDS2BYTES_LOOP:
   for (int i = 0; i < (int)bytestrm_len; i++) {
#pragma HLS loop_tripcount max=372
      if (i % 4 == 0) {
         words_in.read(inval); // or words_in >> inval; or inval = words_in.read();
         tmpword = inval;
      }
      if (!BS) { // shift bytes out in little endian order
         tmpbyte = uint8_t(tmpword);
         tmpword >>= 8;
      } else { // shift bytes out in big endian order
         tmpbyte = uint8_t(tmpword >> ((NB - 1) * 8));
         tmpword <<= 8;
      }
      bytes_out.write(tmpbyte); // or bytes_out << tmpbyte;
   }
}

#endif // BYTESTRM_UTIL_H_ not defined