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

int main (void){

    int cholesky_success = 0;
    float A       [ROWS_COLS_A][ROWS_COLS_A];
    float InverseA[ROWS_COLS_A][ROWS_COLS_A];
    float Ir      [ROWS_COLS_A][ROWS_COLS_A];
    float I_ref   [ROWS_COLS_A][ROWS_COLS_A];

    A[0][0] =  127.0; A[0][1] =  19.0;    A[0][2] =  25.0;
    A[1][0] =   19.0; A[1][1] = 124.0;    A[1][2] =   5.0;
    A[2][0] =   25.0; A[2][1] =   5.0;    A[2][2] = 104.0;

    cholesky_success = cholesky_inverse_top(A,InverseA);

    // Now re-create I: Ir = A * InverseA
    hls::matrix_multiply<hls::NoTranspose,hls::NoTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,float,float>(A, InverseA, Ir);

    printf("A = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, float, hls::NoTranspose>(A, "   ");

    printf("InverseA = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, float, hls::NoTranspose>(InverseA, "   ");

    printf("I reconstructed = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, float, hls::NoTranspose>(Ir, "   ");

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

