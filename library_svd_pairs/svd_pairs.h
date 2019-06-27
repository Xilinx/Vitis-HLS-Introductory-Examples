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

#ifndef SVD_H
#define SVD_H

#include "hls_linear_algebra.h"

const int A_ROWS = 6;
const int A_COLS = 6;

typedef float MATRIX_IN_T;
typedef float MATRIX_OUT_T;

struct SVD_CONFIG : hls::svd_traits<A_ROWS,A_COLS,MATRIX_IN_T,MATRIX_OUT_T>{
  static const int ARCH = 1;
};

void svd_pairs_top(const MATRIX_IN_T A[A_ROWS][A_COLS],
                   MATRIX_OUT_T S[A_ROWS][A_ROWS],
                   MATRIX_OUT_T U[A_ROWS][A_ROWS],
                   MATRIX_OUT_T V[A_COLS][A_COLS]);

#endif


