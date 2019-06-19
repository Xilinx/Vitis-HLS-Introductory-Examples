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

#include "svd.h"

// The top-level functions for each implementation "target": small, balanced, fast, faster, tradeoff
// o Function return must be "void" to use the set_directive_top

void svd_small(const MATRIX_IN_T A[A_ROWS][A_COLS],
               MATRIX_OUT_T S[A_ROWS][A_ROWS],
               MATRIX_OUT_T U[A_ROWS][A_ROWS],
               MATRIX_OUT_T V[A_COLS][A_COLS]) {

  MATRIX_IN_T  a_i[A_ROWS][A_COLS];
  MATRIX_OUT_T s_i[A_ROWS][A_COLS];
  MATRIX_OUT_T u_i[A_ROWS][A_ROWS];
  MATRIX_OUT_T v_i[A_COLS][A_COLS];

  // Copy input data to local memory
  a_row_loop : for (int r=0;r<A_ROWS;r++)
    a_col_loop : for (int c=0;c<A_COLS;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  // Call SVD, using default configuration
  hls::svd<A_ROWS, A_COLS, MATRIX_IN_T, MATRIX_OUT_T>(a_i, s_i, u_i, v_i);
  
  // Copy local memory contents to outputs
  // o Using region to scope application of LOOP_MERGE directive
  copy_ops: {
    #pragma HLS LOOP_MERGE
    s_row_loop : for (int r=0;r<A_ROWS;r++)
      s_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        S[r][c] = s_i[r][c];
    u_row_loop : for (int r=0;r<A_ROWS;r++)
      u_col_loop : for (int c=0;c<A_ROWS;c++)
        #pragma HLS PIPELINE
        U[r][c] = u_i[r][c];
    v_row_loop : for (int r=0;r<A_COLS;r++)
      v_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        V[r][c] = v_i[r][c];
  }
}

void svd_balanced(const MATRIX_IN_T A[A_ROWS][A_COLS],
                  MATRIX_OUT_T S[A_ROWS][A_ROWS],
                  MATRIX_OUT_T U[A_ROWS][A_ROWS],
                  MATRIX_OUT_T V[A_COLS][A_COLS]) {

  MATRIX_IN_T  a_i[A_ROWS][A_COLS];
  MATRIX_OUT_T s_i[A_ROWS][A_COLS];
  MATRIX_OUT_T u_i[A_ROWS][A_ROWS];
  MATRIX_OUT_T v_i[A_COLS][A_COLS];

  // Copy input data to local memory
  a_row_loop : for (int r=0;r<A_ROWS;r++)
    a_col_loop : for (int c=0;c<A_COLS;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  // Call SVD, using default configuration
  hls::svd<A_ROWS, A_COLS, MATRIX_IN_T, MATRIX_OUT_T>(a_i, s_i, u_i, v_i);
  
  // Copy local memory contents to outputs
  // o Using region to scope application of LOOP_MERGE directive
  copy_ops: {
    #pragma HLS LOOP_MERGE
    s_row_loop : for (int r=0;r<A_ROWS;r++)
      s_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        S[r][c] = s_i[r][c];
    u_row_loop : for (int r=0;r<A_ROWS;r++)
      u_col_loop : for (int c=0;c<A_ROWS;c++)
        #pragma HLS PIPELINE
        U[r][c] = u_i[r][c];
    v_row_loop : for (int r=0;r<A_COLS;r++)
      v_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        V[r][c] = v_i[r][c];
  }
}

void svd_fast(const MATRIX_IN_T A[A_ROWS][A_COLS],
              MATRIX_OUT_T S[A_ROWS][A_ROWS],
              MATRIX_OUT_T U[A_ROWS][A_ROWS],
              MATRIX_OUT_T V[A_COLS][A_COLS]) {

  MATRIX_IN_T  a_i[A_ROWS][A_COLS];
  MATRIX_OUT_T s_i[A_ROWS][A_COLS];
  MATRIX_OUT_T u_i[A_ROWS][A_ROWS];
  MATRIX_OUT_T v_i[A_COLS][A_COLS];

  // Copy input data to local memory
  a_row_loop : for (int r=0;r<A_ROWS;r++)
    a_col_loop : for (int c=0;c<A_COLS;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  // Call SVD
  hls::svd_top<A_ROWS, A_COLS, SVD_CONFIG_FAST, MATRIX_IN_T, MATRIX_OUT_T>(a_i, s_i, u_i, v_i);
  
  // Copy local memory contents to outputs
  // o Using region to scope application of LOOP_MERGE directive
  copy_ops: {
    #pragma HLS LOOP_MERGE
    s_row_loop : for (int r=0;r<A_ROWS;r++)
      s_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        S[r][c] = s_i[r][c];
    u_row_loop : for (int r=0;r<A_ROWS;r++)
      u_col_loop : for (int c=0;c<A_ROWS;c++)
        #pragma HLS PIPELINE
        U[r][c] = u_i[r][c];
    v_row_loop : for (int r=0;r<A_COLS;r++)
      v_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        V[r][c] = v_i[r][c];
  }
}

void svd_fast_low_iter(const MATRIX_IN_T A[A_ROWS][A_COLS],
                       MATRIX_OUT_T S[A_ROWS][A_ROWS],
                       MATRIX_OUT_T U[A_ROWS][A_ROWS],
                       MATRIX_OUT_T V[A_COLS][A_COLS])  {

  MATRIX_IN_T  a_i[A_ROWS][A_COLS];
  MATRIX_OUT_T s_i[A_ROWS][A_COLS];
  MATRIX_OUT_T u_i[A_ROWS][A_ROWS];
  MATRIX_OUT_T v_i[A_COLS][A_COLS];

  // Copy input data to local memory
  a_row_loop : for (int r=0;r<A_ROWS;r++)
    a_col_loop : for (int c=0;c<A_COLS;c++)
      #pragma HLS PIPELINE
      a_i[r][c] = A[r][c];
  
  // Call SVD
  hls::svd_top<A_ROWS, A_COLS, SVD_CONFIG_FAST_LOW_ITER, MATRIX_IN_T, MATRIX_OUT_T>(a_i, s_i, u_i, v_i);
  
  // Copy local memory contents to outputs
  // o Using region to scope application of LOOP_MERGE directive
  copy_ops: {
    #pragma HLS LOOP_MERGE
    s_row_loop : for (int r=0;r<A_ROWS;r++)
      s_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        S[r][c] = s_i[r][c];
    u_row_loop : for (int r=0;r<A_ROWS;r++)
      u_col_loop : for (int c=0;c<A_ROWS;c++)
        #pragma HLS PIPELINE
        U[r][c] = u_i[r][c];
    v_row_loop : for (int r=0;r<A_COLS;r++)
      v_col_loop : for (int c=0;c<A_COLS;c++)
        #pragma HLS PIPELINE
        V[r][c] = v_i[r][c];
  }
}


