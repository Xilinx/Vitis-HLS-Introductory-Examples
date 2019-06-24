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
#ifndef NCO_H
#define NCO_H

#include "ap_int.h"
#include "hls_dsp.h"
#include <hls_stream.h>

// Define template parameters
const int ACCUM_WIDTH = 8;
const int PHASE_ANGLE_WIDTH = 8;
const int SUPER_SAMPLE_RATE = 4;
const int OUTPUT_WIDTH = 12;
// Define implementation of complex multiplier structures within the NCO
typedef hls::NcoDualOutputCmpyFiveMult DUAL_OUTPUT_CMPY_IMPL;
typedef hls::NcoSingleOutputCmpyThreeMult SINGLE_OUTPUT_CMPY_IMPL;
typedef hls::NcoSingleOutputCmpyThreeMult SINGLE_OUTPUT_NEG_CMPY_IMPL;

// Define initial phase increment and offset values to be passed as constructor arguments
const ap_uint<ACCUM_WIDTH> INIT_PINC = 100;
const ap_uint<ACCUM_WIDTH> INIT_POFF = 50;

void nco_top(hls::stream< ap_uint<ACCUM_WIDTH > > &pinc,
             hls::stream< ap_uint<ACCUM_WIDTH > > &poff,
             hls::stream< hls::t_nco_output_data<SUPER_SAMPLE_RATE, OUTPUT_WIDTH> > &outputVal);

#endif

