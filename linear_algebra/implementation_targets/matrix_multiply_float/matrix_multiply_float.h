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
 *     (c) Copyright 2013-2019 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#ifndef MATRIX_MULTIPLY_H
#define MATRIX_MULTIPLY_H

#include "hls_linear_algebra.h"

const unsigned A_ROWS = 8;
const unsigned A_COLS = 8;
const unsigned B_ROWS = 8;
const unsigned B_COLS = 8;
const unsigned C_ROWS = A_ROWS;
const unsigned C_COLS = B_COLS;

// Define implementation type
// typedef float MATRIX_T;
typedef std::complex<float> MATRIX_T;

// Define configuration class for each implementation "target"
struct MULT_CONFIG_SMALL: hls::matrix_multiply_traits<hls::NoTranspose,hls::NoTranspose,A_ROWS,A_COLS,B_ROWS,B_COLS,MATRIX_T, MATRIX_T>{
  static const int INNER_II = 4;
};
struct MULT_CONFIG_FAST: hls::matrix_multiply_traits<hls::NoTranspose,hls::NoTranspose,A_ROWS,A_COLS,B_ROWS,B_COLS,MATRIX_T, MATRIX_T>{
  static const int UNROLL_FACTOR = 4;
};
struct MULT_CONFIG_FASTER: hls::matrix_multiply_traits<hls::NoTranspose,hls::NoTranspose,A_ROWS,A_COLS,B_ROWS,B_COLS,MATRIX_T, MATRIX_T>{
  static const int ARCH = 3;
};

// Declare top-level functions for each implementation "target"
void matrix_multiply_small(const MATRIX_T A [A_ROWS][A_COLS],
                           const MATRIX_T B [B_ROWS][B_COLS],
                                 MATRIX_T C [C_ROWS][C_COLS]);
void matrix_multiply_default(const MATRIX_T A [A_ROWS][A_COLS],
                             const MATRIX_T B [B_ROWS][B_COLS],
                                   MATRIX_T C [C_ROWS][C_COLS]);
void matrix_multiply_fast(const MATRIX_T A [A_ROWS][A_COLS],
                          const MATRIX_T B [B_ROWS][B_COLS],
                                MATRIX_T C [C_ROWS][C_COLS]);
void matrix_multiply_faster(const MATRIX_T A [A_ROWS][A_COLS],
                            const MATRIX_T B [B_ROWS][B_COLS],
                                  MATRIX_T C [C_ROWS][C_COLS]);


#endif

