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

#include "qrf_alt.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

int main (void){

  float A    [A_ROWS][A_COLS];
  float Q    [A_ROWS][A_ROWS];
  float R    [A_ROWS][A_COLS];
  float Ar   [A_ROWS][A_COLS]; // Reconstructed A
  float Ir   [A_ROWS][A_ROWS]; // Reconstructed I
  float I_ref[A_ROWS][A_ROWS];

  // Set up example data
  float val = -(A_ROWS*A_COLS)/2;
  for (int r=0;r<A_ROWS;r++) {
    for (int c=0;c<A_COLS;c++) {
      A[r][c] = val;
      val++;
    }
  }

  // Call wrapped QR Factorization - Q is being output such that Q*R = A
  qrf_alt_top(A,Q,R);

  // Zero lower-triangular elements of R to permit reconstruction of A.
  // The QRF function does not zero these elements.
  for (int r = 0; r < A_ROWS; r++) {
    for (int c = 0; c < A_COLS; c++) {
      if (c < r) {
        R[r][c] = 0.0;
      }
    }
  }

  // Reconstruct A with Q*R
  hls::matrix_multiply<hls::NoTranspose, hls::NoTranspose, A_ROWS, A_ROWS, A_ROWS, A_COLS, A_ROWS, A_COLS, float, float >(Q, R, Ar);

  // Reconstruct I with Q*Q'
  hls::matrix_multiply<hls::NoTranspose, hls::ConjugateTranspose, A_ROWS, A_ROWS, A_ROWS, A_ROWS, A_ROWS, A_ROWS, float, float >(Q, Q, Ir);

  // Print results
  printf("A = \n");
  hls::print_matrix<A_ROWS, A_COLS, float, hls::NoTranspose>(A, "   ");

  printf("Q = \n");
  hls::print_matrix<A_ROWS, A_ROWS, float, hls::NoTranspose>(Q, "   ");

  printf("R = \n");
  hls::print_matrix<A_ROWS, A_COLS, float, hls::NoTranspose>(R, "   ");

  printf("A reconstructed = \n");
  hls::print_matrix<A_ROWS, A_COLS, float, hls::NoTranspose>(Ar, "   ");

  printf("I reconstructed = \n");
  hls::print_matrix<A_ROWS, A_ROWS, float, hls::NoTranspose>(Ir, "   ");

  // Generate reference I
  for (int r=0;r<A_ROWS;r++) {
    for (int c=0;c<A_COLS;c++) {
      I_ref[r][c] = 0.0;
      if ( r == c ) {
        I_ref[r][c] = 1.0;
      }
    }
  }
  // Generate error ratios and compare against threshold value
  // - LAPACK error measurement method
  // - Threshold taken from LAPACK test functions
  int fail = 0;
  if ( difference_ratio<A_ROWS, A_COLS>(Ar,A) > 30.0 ) {
    fail = 1;
  }
  if ( difference_ratio<A_ROWS, A_ROWS>(Ir,I_ref) > 30.0 ) {
    fail = 1;
  }
  return(fail);
}

