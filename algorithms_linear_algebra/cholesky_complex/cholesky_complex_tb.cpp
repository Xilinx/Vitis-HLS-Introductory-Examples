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

#include "cholesky_complex.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

int main (void){

    int cholesky_success = 0;
    MATRIX_IN_T  A [ROWS_COLS_A][ROWS_COLS_A];
    MATRIX_OUT_T L [ROWS_COLS_A][ROWS_COLS_A];
    MATRIX_IN_T  Ar[ROWS_COLS_A][ROWS_COLS_A];

    A[0][0].real(82.0); A[0][0].imag(0.0) ; A[0][1].real(2.0) ; A[0][1].imag(9.0) ; A[0][2].real(-2.0) ; A[0][2].imag(-22.0);
    A[1][0].real(2.0) ; A[1][0].imag(-9.0); A[1][1].real(92.0); A[1][1].imag(0.0) ; A[1][2].real(6.0)  ; A[1][2].imag(1.0);
    A[2][0].real(-2.0); A[2][0].imag(22.0); A[2][1].real(6.0) ; A[2][1].imag(-1.0); A[2][2].real(127.0); A[2][2].imag(0.0);

    cholesky_success = cholesky_complex_top(A,L);

    // Now re-create A: Ar = L * L'
    hls::matrix_multiply<hls::NoTranspose,hls::ConjugateTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,MATRIX_OUT_T,MATRIX_IN_T>(L, L, Ar);

    printf("A = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, hls::NoTranspose>(A, "   ");

    printf("L = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_OUT_T, hls::NoTranspose>(L, "   ");

    printf("A reconstructed = \n");
    hls::print_matrix<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, hls::NoTranspose>(Ar, "   ");

    // Generate error ratio and compare against threshold value
    // - LAPACK error measurement method
    // - Threshold taken from LAPACK test functions
    int fail = 0;
    if ( difference_ratio<ROWS_COLS_A, ROWS_COLS_A>(Ar,A) > 30.0 ) {
      fail = 1;
    }
    return (cholesky_success||fail);
}

