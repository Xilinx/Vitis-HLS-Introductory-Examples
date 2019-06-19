/*******************************************************************************
Vendor: Xilinx 
Associated Filename: fp_mul_pow2.c
Purpose:Vivado HLSfp_mul_pow2 example project
Revision History: Feb 21, 2012 - initial release
                                                
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
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include "fp_mul_pow2.h"

#ifdef ABS
#undef ABS
#endif
#define ABS(n) ((n < 0) ? -n : n)

float float_mul_pow2(float x, int8_t n)
{
#pragma AP inline // Always inline this function
   float_num_t x_num, prod;

   x_num.fp_num = x;
#ifndef AESL_FP_MATH_NO_BOUNDS_TESTS
   if (x_num.bexp == 0xFF || x_num.bexp == 0) // pass through NaN, INF & denorm
      prod.fp_num = x_num.fp_num;
   else if (n >= 0 && x_num.bexp >= 255 - n) { // detect and handle overflow
      prod.sign = x_num.sign; //
      prod.bexp = 0xFF;       // +/-INF
      prod.mant = 0;          //
   } else if (n < 0 && x_num.bexp <= ABS(n)) { // handle underflow (doesn't gen denorms)
      prod.sign = x_num.sign; //
      prod.bexp = 0;          // +/-ZERO
      prod.mant = 0;          //
   } else
#endif // AESL_FP_MATH_NO_BOUNDS_TESTS not defined
   {
      prod.sign = x_num.sign;
      prod.bexp = x_num.bexp + n;
      prod.mant = x_num.mant;
   }
   return prod.fp_num;
}

double double_mul_pow2(double x, int16_t n)
{
#pragma AP inline // Always inline this function
   double_num_t x_num, prod;

   x_num.fp_num = x;
#ifndef AESL_FP_MATH_NO_BOUNDS_TESTS
   if (x_num.bexp == 0xFF || x_num.bexp == 0) // pass through NaN, INF & denorm
      prod.fp_num = x_num.fp_num;
   else if (n >= 0 && x_num.bexp >= 2047 - n) { // detect and handle overflow
      prod.sign = x_num.sign; //
      prod.bexp = 0x7FF;      // +/-INF
      prod.mant = 0;          //
   } else if (n < 0 && x_num.bexp <= ABS(n)) { // handle underflow (doesn't gen denorms)
      prod.sign = x_num.sign; //
      prod.bexp = 0;          // +/-ZERO
      prod.mant = 0;          //
   } else
#endif // AESL_FP_MATH_NO_BOUNDS_TESTS not defined
   {
      prod.sign = x_num.sign;
      prod.bexp = x_num.bexp + n;
      prod.mant = x_num.mant;
   }
   return prod.fp_num;
}

