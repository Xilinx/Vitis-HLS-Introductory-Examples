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

#include "matrix_multiply_fixed.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

// Dummy top-level for testbench. 
// o A different synthesis top-level is selected for each solution by using set_directive_top
// o DESIGN_TOP is the function name specified as the project top (set_top) which each solution
//   points to a different implementation top-level function.
void DESIGN_TOP(const MATRIX_T A [A_ROWS][A_COLS],
                const MATRIX_T B [B_ROWS][B_COLS],
                      MATRIX_T C [C_ROWS][C_COLS]){
  // matrix_multiply_small(A, B, C);
  matrix_multiply_default(A, B, C);
  // matrix_multiply_fast(A, B, C);
  // matrix_multiply_full(A, B, C);
}

int main (void){

  MATRIX_T A[A_ROWS][A_COLS];
  MATRIX_T B[B_ROWS][B_COLS];
  MATRIX_T C[C_ROWS][C_COLS];
  
  // A content, simple count
  int cnt=1;
  for (int i=0;i<A_ROWS;i++)
    for (int j=0;j<A_COLS;j++) {
      A[i][j] = cnt;
      cnt++;
    }
  
  // Create identity for B
  for (int i=0;i<B_ROWS;i++)
    for (int j=0;j<B_COLS;j++)
      if (i==j)
        B[i][j] = 1;
      else
        B[i][j] = 0;
      
  for (int i=0;i<10;i++)
    DESIGN_TOP(A, B, C);

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

