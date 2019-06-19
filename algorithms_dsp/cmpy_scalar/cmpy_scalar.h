/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2014-2019 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/
#ifndef CMPY_SCALAR_H
#define CMPY_SCALAR_H

#include "hls_dsp.h"

//Configuration parameters for this instance.
typedef hls::CmpyThreeMult ARCH;
//ROUND_MODE Values : AP_RND, AP_RND_ZERO, AP_RND_MIN_INF, AP_RND_INF, AP_RND_CONV, AP_TRN, AP_TRN_ZERO
//OVERFLOW_MODE Values : AP_SAT, AP_SAT_ZERO, AP_SAT_SYM, AP_WRAP, AP_WRAP_SM

const int INPUT_WIDTH                = 2;
const int INPUT_INTEGER_BITS         = 1;
const int INPUT_ROUND_MODE           = 5;//AP_TRN, etc;
const int INPUT_OVERFLOW_MODE        = 3;//AP_WRAP, etc;
const int INPUT_SATURATION_BITS      = 0;

const int OUTPUT_WIDTH               = 4;
const int OUTPUT_INTEGER_BITS        = 2;
const int OUTPUT_ROUND_MODE          = 0;//AP_TRN, etc;
const int OUTPUT_OVERFLOW_MODE       = 3;//AP_WRAP, etc;
const int OUTPUT_SATURATION_BITS     = 0;

//Interface types
typedef ap_fixed<
  INPUT_WIDTH,
  INPUT_INTEGER_BITS,
  (ap_q_mode)INPUT_ROUND_MODE,
  (ap_o_mode)INPUT_OVERFLOW_MODE,
  INPUT_SATURATION_BITS> t_input_scalar;

typedef ap_fixed<
  OUTPUT_WIDTH,
  OUTPUT_INTEGER_BITS,
  (ap_q_mode)OUTPUT_ROUND_MODE,
  (ap_o_mode)OUTPUT_OVERFLOW_MODE,
  OUTPUT_SATURATION_BITS> t_output_scalar;

void cmpy_scalar_top(const t_input_scalar  &ar, const t_input_scalar &ai,
                     const t_input_scalar  &br, const t_input_scalar &bi,
                     t_output_scalar &pr, t_output_scalar &pi);

#endif

