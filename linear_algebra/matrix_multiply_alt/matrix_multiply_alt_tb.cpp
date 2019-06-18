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

#include "matrix_multiply_alt.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

int main (void){

  MATRIX_T A[A_ROWS][A_COLS];
  MATRIX_T B[B_ROWS][B_COLS];
  MATRIX_T C[C_ROWS][C_COLS];

  A[0][0] = 1.0;   A[0][1] = 2.0;   A[0][2] = 3.0;  A[0][3] = 4.0;
  A[1][0] = 5.0;   A[1][1] = 6.0;   A[1][2] = 7.0;  A[1][3] = 8.0;
  A[2][0] = 9.0;   A[2][1] = 10.0;  A[2][2] = 11.0; A[2][3] = 12.0;
  A[3][0] = 13.0;  A[3][1] = 14.0;  A[3][2] = 15.0; A[3][3] = 16.0;

  B[0][0] = 1.0;  B[0][1] = 0.0;  B[0][2] = 0.0; B[0][3] = 0.0;
  B[1][0] = 0.0;  B[1][1] = 1.0;  B[1][2] = 0.0; B[1][3] = 0.0;
  B[2][0] = 0.0;  B[2][1] = 0.0;  B[2][2] = 1.0; B[2][3] = 0.0;
  B[3][0] = 0.0;  B[3][1] = 0.0;  B[3][2] = 0.0; B[3][3] = 1.0;

  matrix_multiply_alt_top(A, B, C);

  printf("A = \n");
  hls::print_matrix<A_ROWS, A_COLS, MATRIX_T, hls::NoTranspose>(A, "   ");

  printf("B = \n");
  hls::print_matrix<B_ROWS, B_COLS, MATRIX_T, hls::NoTranspose>(B, "   ");

  printf("C = \n");
  hls::print_matrix<C_ROWS, C_COLS, MATRIX_T, hls::NoTranspose>(C, "   ");

  // Generate error ratio and compare against threshold value
  // - LAPACK error measurement method
  // - Threshold taken from LAPACK test functions
  if ( difference_ratio<A_ROWS, A_COLS>(A,C) > 30.0 ) {
    return(1);
  }
  return(0);
}

