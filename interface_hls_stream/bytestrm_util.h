/*******************************************************************************
Vendor: Xilinx
Associated Filename: bytestrm_util.h
Purpose:Vivado HLS Coding Style example
Device: All
Revision History: May 30, 2008 - initial release

*******************************************************************************
#-  (c) Copyright 2011-2019 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer asresultes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/

#ifndef BYTESTRM_UTIL_H_
#define BYTESTRM_UTIL_H_

#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace std;

#define MAX_BYTESTRM_LEN 1488

// Convert a stream of bytes (uint8_t) to a stream of arbitrary width
// (on byte boundaries) words
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

