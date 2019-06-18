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

#include "cholesky_inverse.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

// Dummy top-level for testbench. 
// o A different synthesis top-level is selected for each solution by using set_directive_top
// o DESIGN_TOP is the function name specified as the project top (set_top) which each solution
//   points to a different implementation top-level function.
void DESIGN_TOP(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A],
                int& inverse_OK) {
  // cholesky_inverse_small(A,InverseA,inverse_OK);
  cholesky_inverse_balanced(A,InverseA,inverse_OK);
  // cholesky_inverse_fast(A,InverseA,inverse_OK);
}

int main (void){

    int cholesky_success = 0;
    MATRIX_IN_T  A       [ROWS_COLS_A][ROWS_COLS_A];
    MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A];
    MATRIX_OUT_T Ir      [ROWS_COLS_A][ROWS_COLS_A];
    MATRIX_OUT_T I_ref   [ROWS_COLS_A][ROWS_COLS_A];
    
    // Set a default identity matrix
    for (int i=0;i<ROWS_COLS_A;i++)
      for (int j=0;j<ROWS_COLS_A;j++) {
        if (i==j)
          A[i][j] = 1;
        else
          A[i][j] = 0;
      }
    // Overlay a symmetric positive definite 6x6 matrix in the centre
    int k = ROWS_COLS_A/2 - 3;
    A[0+k][0+k] = 115.0; A[0+k][1+k] =  -7.0; A[0+k][2+k] = -12.0; A[0+k][3+k] = -14.0; A[0+k][4+k] =  26.0; A[0+k][5+k] =   0.0;
    A[1+k][0+k] =  -7.0; A[1+k][1+k] = 120.0; A[1+k][2+k] =  17.0; A[1+k][3+k] = -23.0; A[1+k][4+k] =  -6.0; A[1+k][5+k] =  -3.0;
    A[2+k][0+k] = -12.0; A[2+k][1+k] =  17.0; A[2+k][2+k] = 124.0; A[2+k][3+k] =  -2.0; A[2+k][4+k] = -12.0; A[2+k][5+k] =   0.0;
    A[3+k][0+k] = -14.0; A[3+k][1+k] = -23.0; A[3+k][2+k] =  -2.0; A[3+k][3+k] = 108.0; A[3+k][4+k] =  -5.0; A[3+k][5+k] =  -6.0;
    A[4+k][0+k] =  26.0; A[4+k][1+k] =  -6.0; A[4+k][2+k] = -12.0; A[4+k][3+k] =  -5.0; A[4+k][4+k] = 127.0; A[4+k][5+k] = -10.0;
    A[5+k][0+k] =   0.0; A[5+k][1+k] =  -3.0; A[5+k][2+k] =   0.0; A[5+k][3+k] =  -6.0; A[5+k][4+k] = -10.0; A[5+k][5+k] = 119.0;
    
    for (int i=0;i<10;i++)
      DESIGN_TOP(A,InverseA,cholesky_success);
    
    // Now re-create I: Ir = A * InverseA
    hls::matrix_multiply<hls::NoTranspose,hls::NoTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,MATRIX_OUT_T,MATRIX_OUT_T>(A, InverseA, Ir);

    printf("A = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, hls::NoTranspose>(A, "   ");

    printf("InverseA = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_OUT_T, hls::NoTranspose>(InverseA, "   ");

    printf("I reconstructed = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_OUT_T, hls::NoTranspose>(Ir, "   ");

    // Generate reference I
    for (int r=0;r<ROWS_COLS_A;r++) {
      for (int c=0;c<ROWS_COLS_A;c++) {
        I_ref[r][c] = 0.0;
        if ( r == c ) {
          I_ref[r][c] = 1.0;
        }
      }
    }
    // Generate error ratio and compare against threshold value
    // - LAPACK error measurement method
    // - Threshold taken from LAPACK test functions
    int fail = 0;
    if ( difference_ratio<ROWS_COLS_A, ROWS_COLS_A>(Ir,I_ref) > 30.0 ) {
      fail = 1;
    }
    return (cholesky_success||fail);
}

