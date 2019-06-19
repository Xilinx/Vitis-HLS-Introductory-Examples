/*******************************************************************************
Vendor: Xilinx
Associated Filename: bytestrm_dwordproc_test.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include "bytestrm_dwordproc.h"

#define TESTDATA_LEN (DECIMATION_RATE * 64)

int main()
{
   int32_t testdata[TESTDATA_LEN];
   int32_t hw_result;
   int64_t sw_result;
   int i, j, err_cnt = 0;

   stream<uint8_t> bytestr_out;
   stream<uint8_t> bytestr_in;

   for (i = 0; i < TESTDATA_LEN; i++) {
      testdata[i] = rand();

      for (j = 0; j < sizeof(uint32_t); j++) {
         bytestr_out << uint8_t((testdata[i] >> (j * 8)) & 0xFF);
      }
   }

   bytestrm_dwordproc(bytestr_in, bytestr_out, TESTDATA_LEN * sizeof(int));

   for (i = 0; i < TESTDATA_LEN / DECIMATION_RATE; i++) {
      for (j = 0; j < DECIMATION_RATE; j++) {
         uint8_t bytein;
         bytestr_in >> bytein;
         hw_result = ((hw_result >> 8) & (0x00FFFFFF)) | (bytein << 24);
      }
      sw_result = 0;
      for (j = 0; j < DECIMATION_RATE; j++) {
         sw_result += testdata[i * DECIMATION_RATE + j];
      }
      sw_result >>= LOG2_DECIMATION_RATE;

      if (hw_result != sw_result) {
         err_cnt++;
#if 1
         printf("!!! MISMATCH @ [%3d] - Expected: %08X\t Got: %08X\n",
        		 i, uint32_t(sw_result), uint32_t(hw_result));
#endif
      }
   }

   if (err_cnt) {
      printf("!!! ERRORS ENCOUNTERED - Test Fails !!!\n");
   } else {
      printf("*** Test Passes ***\n");
   }

   if (err_cnt)
       return 1;
   else
       return 0;
}

