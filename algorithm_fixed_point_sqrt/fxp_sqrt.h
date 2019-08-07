/*******************************************************************************
Vendor: Xilinx 
Associated Filename: fxp_sqrt.h
Purpose:Vivado HLSfxp_sqrt example project
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

#ifndef __FXP_SQRT_H__
#define __FXP_SQRT_H__

#include <cassert>
#include <ap_fixed.h>
using namespace std;

// Fixed point square-root template
//
// Basic usage: fxp_sqrt(root_var, radicand_var);
//          or: fxp_sqrt<W2,IW2,W1,IW1>(root_var, radicand_var);
// where root_var and radicand_var are ap_ufixed<> variables and W2 & IW2
// are the total and iteger widths for root_var and W1/IW1 those for 
// radicand_var.
//
// Despcription:
// This template function implements a non-restoring algorithm to calculate
// the square-root of a fixed point variable (ap_ufixed<>) of arbitrary
// format and assign it to another fixed point vaiable of arbitrary format.
//
// The template parameters specify the ap_ufixed<> width parameters to be used
// for input and output formats (see documentation for ap_fixed<> for details).
// These parameters do not need to be explicitly specified, as they will be
// inferred by the types passed to the function call.
//
// As this function is only intended for use with real numbers, the input 
// and output variables are expected to be unsigned, i.e. ap_ufixed<>.  If
// signed types are passed, it is the user's responsibility to ensure the
// input variable does not go negative, as no checks are made for such; 
// also note that passing a signed variable may result in slighty sub-optimal
// HLS due to a wider format than necessary.  Explicit casting is preferred.
//
// This function returns a precise (rounded to one ulp) result
// as long as the output format has at least half (rounded up) as many
// fractional bits as the input format.  The output format must have enough
// whole (integer) bit to hold thd full range of the result (i.e. half 
// rounded up), otherwise an assertion failure will result at runtime (of SW
// model and/or RTL co-simulation).
//
// Note that integer square-roots may be accomplished with this function by
// use of ap_ufixed<> intermediate variables with no fractional portion, e.g.
//    uint16_t a;
//    uint8_t a_sqrt;
//    ...
//    ap_ufixed<16,16> a_fxp = ap_ufixed<16,16>(a);
//    ap_ufixed<8,8> a_sqrt_fxp;
//    fxp_sqrt(a_sqrt_fxp, a_fxp);
//    a_sqrt = a_sqrt_fxp.toint();
//
// Which should result in no more hardware area than having an integer-
// specific version of this function.

template <int W2, int IW2, int W1, int IW1>
void fxp_sqrt(ap_ufixed<W2,IW2>& result, ap_ufixed<W1,IW1>& in_val)
{
   enum { QW = (IW1+1)/2 + (W2-IW2) + 1 }; // derive max root width
   enum { SCALE = (W2 - W1) - (IW2 - (IW1+1)/2) }; // scale (shift) to adj initial remainer value
   enum { ROOT_PREC = QW - (IW1 % 2) };
   assert((IW1+1)/2 <= IW2); // Check that output format can accommodate full result

   ap_uint<QW> q      = 0;   // partial sqrt
   ap_uint<QW> q_star = 0;   // diminished partial sqrt
   ap_int<QW+2> s; // scaled remainder initialized to extracted input bits
   if (SCALE >= 0)
      s = in_val.range(W1-1,0) << (SCALE);
   else
      s = ((in_val.range(W1-1,0) >> (0 - (SCALE + 1))) + 1) >> 1;

   // Non-restoring square-root algorithm
   for (int i = 0; i <= ROOT_PREC; i++) {
      if (s >= 0) {
         s = 2 * s - (((ap_int<QW+2>(q) << 2) | 1) << (ROOT_PREC - i));
         q_star = q << 1;
         q = (q << 1) | 1;
      } else {
         s = 2 * s + (((ap_int<QW+2>(q_star) << 2) | 3) << (ROOT_PREC - i));
         q = (q_star << 1) | 1;
         q_star <<= 1;
      }
   }
   // Round result by "extra iteration" method
   if (s > 0)
      q = q + 1;
   // Truncate excess bit and assign to output format
   result.range(W2-1,0) = ap_uint<W2>(q >> 1);
}

#endif //__FXP_SQRT_H__ not defined