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
 *     (c) Copyright 2015-2019 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "cholesky.h"

// The top-level functions for each implementation "target": small, balanced, fast, faster, tradeoff
// o Function return must be "void" to use the set_directive_top

void cholesky_small(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                    MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A],
                    int& passfail){
  
  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];
  
  // Copy input data to local memory
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
    
  // Call Cholesky
  passfail = hls::cholesky_top<LOWER_TRIANGULAR, ROWS_COLS_A, CHOLESKY_CONFIG_SMALL, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i);
  
  // Copy local memory contents to output
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      L[r][c] = l_i[r][c];
}

void cholesky_balanced(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                       MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A],
                       int& passfail){
  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];
  
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
    
  // Call Cholesky, using defaults
  passfail = hls::cholesky<LOWER_TRIANGULAR, ROWS_COLS_A, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i);
  
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      L[r][c] = l_i[r][c];
}

void cholesky_alt_balanced(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                           MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A],
                           int& passfail){
  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];
  
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  passfail = hls::cholesky_top<LOWER_TRIANGULAR, ROWS_COLS_A, CHOLESKY_CONFIG_ALT_BALANCED, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i);
  
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      L[r][c] = l_i[r][c];
}

void cholesky_fast(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                   MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A],
                   int& passfail){
  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];
  
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  passfail = hls::cholesky_top<LOWER_TRIANGULAR, ROWS_COLS_A, CHOLESKY_CONFIG_FAST, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i);
  
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      L[r][c] = l_i[r][c];
}

void cholesky_faster(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                     MATRIX_OUT_T L[ROWS_COLS_A][ROWS_COLS_A],
                     int& passfail){
  MATRIX_IN_T  a_i[ROWS_COLS_A][ROWS_COLS_A];
  MATRIX_OUT_T l_i[ROWS_COLS_A][ROWS_COLS_A];
  
  a_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    a_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  passfail = hls::cholesky_top<LOWER_TRIANGULAR, ROWS_COLS_A, CHOLESKY_CONFIG_FASTER, MATRIX_IN_T, MATRIX_OUT_T>(a_i, l_i);
  
  l_row_loop : for (int r=0;r<ROWS_COLS_A;r++)
    l_col_loop : for (int c=0;c<ROWS_COLS_A;c++)
      #pragma HLS PIPELINE
      L[r][c] = l_i[r][c];
}



