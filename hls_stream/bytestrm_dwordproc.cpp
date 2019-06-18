/*******************************************************************************
Vendor: Xilinx
Associated Filename: bytestrm_dwordproc.cpp
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

#include "bytestrm_dwordproc.h"

using namespace hls;

// Local function prototypes
static void decimate_strm(stream<int32_t> &strm_out,
      stream<int32_t> &strm_in, uint16_t strm_len);

// The top-level function
void bytestrm_dwordproc(
      hls::stream<uint8_t> &strm_out,
      hls::stream<uint8_t> &strm_in,
      uint16_t strm_len
      )
{
//#pragma HLS interface ap_hs port=&strm_out,&strm_in
#pragma HLS interface axis port=strm_in
#pragma HLS interface axis port=strm_out
   unsigned i;
   uint32_t tmp;
   stream<int32_t> dwordstrm_in;
   stream<int32_t> dwordstrm_out;
#pragma HLS stream depth=4 variable=dwordstrm_in // Set depth to non-default value

#pragma HLS dataflow

// manually re-distributing allows proper streaming and internal FIFO depths can be minimal
   uint16_t bytes_in_len = strm_len;
   uint16_t dwords_in_len = strm_len / 4;
   uint16_t bytes_out_len = (dwords_in_len / DECIMATION_RATE) * 4;

   strm_bytes2words<int32_t, sizeof(int32_t), false>(dwordstrm_in,
         strm_in, bytes_in_len);

   decimate_strm(dwordstrm_out, dwordstrm_in, dwords_in_len);

   strm_words2bytes<int32_t, sizeof(int32_t), false>(strm_out,
         dwordstrm_out, bytes_out_len);
}

// Local function definitions
static void decimate_strm(
      stream<int32_t> &strm_out,
      stream<int32_t> &strm_in,
      uint16_t strm_len)
{
   unsigned i, j;
   int64_t acc;
   int32_t inval;
STREAM_LOOP:
   for (i = 0; i < strm_len / DECIMATION_RATE; i++) {
#pragma HLS loop_tripcount max=93 // max strm_in_len / decimation rate
      acc = 0;
DECIMATE_LOOP:
      for (j = 0; j < DECIMATION_RATE; j++) {
#pragma HLS pipeline
         acc += strm_in.read(); // N.B. - this would be two statements w/ other read methods:
         //strm_in.read(inval); // or: strm_in >> inval;
         //acc += inval;
         if (j == DECIMATION_RATE - 1)
            strm_out << int32_t(acc >> LOG2_DECIMATION_RATE);
      }
   }
}

