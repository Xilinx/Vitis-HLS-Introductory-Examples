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

#include "cholesky.h"

// The top-level function to synthesize
//
int cholesky_top(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A]){

  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];

  // Copy input data to local memory
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++) {
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++) {
      a_i[r][c] = A[r][c];
    }
  }

  // Call Cholesky
  if ( hls::cholesky<LOWER_TRIANGULAR, ROWS_COLS_A, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i) ) {
    // Error - matrix was not symmetric positive definite
    return 1;
  }

  // Copy local memory contents to output
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++) {
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++) {
      L[r][c] = l_i[r][c];
    }
  }

  return 0;
}


