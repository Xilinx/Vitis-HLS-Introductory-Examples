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
#ifndef AWGN_H
#define AWGN_H

#include "ap_int.h"
#include "hls_dsp.h"
#include <hls_stream.h>

//Configuration parameters
const int OUTPUT_WIDTH = 16;
const int LFSR_WIDTH = hls::awgn<OUTPUT_WIDTH>::LFSR_WIDTH;
const ap_uint<LFSR_WIDTH> SEED = ap_uint<LFSR_WIDTH>("0123456789ABCDEF123456789ABCDEF0",16);

typedef hls::awgn<OUTPUT_WIDTH>::t_input_scale t_snr;

void awgn_top(hls::stream< t_snr > &snr,
              hls::stream< ap_int<OUTPUT_WIDTH> > &noise);

#endif

