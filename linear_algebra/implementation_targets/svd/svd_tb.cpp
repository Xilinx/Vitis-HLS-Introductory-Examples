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
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"

// Dummy top-level for testbench. 
// o A different synthesis top-level is selected for each solution by using set_directive_top
// o DESIGN_TOP is the function name specified as the project top (set_top) which each solution
//   points to a different implementation top-level function.
void DESIGN_TOP(const MATRIX_IN_T A[A_ROWS][A_COLS],
                     MATRIX_OUT_T S[A_ROWS][A_ROWS],
                     MATRIX_OUT_T U[A_ROWS][A_ROWS],
                     MATRIX_OUT_T V[A_COLS][A_COLS]){
  // svd_small(A,S,U,V);
  svd_balanced(A,S,U,V);
  svd_fast(A,S,U,V);
  // svd_faster(A,S,U,V);
  // svd_fast_low_iter(A,S,U,V);
}

int main (void){

    MATRIX_IN_T  A    [A_ROWS][A_COLS];
    MATRIX_OUT_T U    [A_ROWS][A_ROWS];
    MATRIX_OUT_T S    [A_ROWS][A_COLS];
    MATRIX_OUT_T V    [A_COLS][A_COLS];
    MATRIX_OUT_T US   [A_ROWS][A_COLS]; // Partially reconstructed A
    MATRIX_OUT_T USV  [A_ROWS][A_COLS]; // Reconstructed A
    MATRIX_OUT_T Ir_U [A_ROWS][A_COLS]; // Reconstructed I from U
    MATRIX_OUT_T Ir_V [A_ROWS][A_COLS]; // Reconstructed I from V
    MATRIX_OUT_T I_ref[A_ROWS][A_COLS];

    // Set up example data
    float val = -(A_ROWS*A_COLS)/2;
    for (int r=0;r<A_ROWS;r++) {
      for (int c=0;c<A_COLS;c++) {
        A[r][c] = val;
        val++;
      }
    }

    // Call wrapped SVD
    for (int i=0;i<5;i++)
      DESIGN_TOP(A,S,U,V);

    // Reconstruct A with U*S*V
    hls::matrix_multiply<hls::NoTranspose, hls::NoTranspose, A_ROWS, A_ROWS, A_ROWS, A_COLS, A_ROWS, A_COLS, MATRIX_OUT_T, MATRIX_OUT_T>(U, S, US);
    hls::matrix_multiply<hls::NoTranspose, hls::ConjugateTranspose, A_ROWS, A_COLS, A_COLS, A_COLS, A_ROWS, A_COLS, MATRIX_OUT_T, MATRIX_OUT_T>(US, V, USV);

    // Reconstruct I with U*U'
    hls::matrix_multiply<hls::NoTranspose, hls::ConjugateTranspose, A_ROWS, A_ROWS, A_ROWS, A_ROWS, A_ROWS, A_ROWS, MATRIX_OUT_T, MATRIX_OUT_T>(U, U, Ir_U);

    // Reconstruct I with V*V'
    hls::matrix_multiply<hls::NoTranspose, hls::ConjugateTranspose, A_COLS, A_COLS, A_COLS, A_COLS, A_COLS, A_COLS, MATRIX_OUT_T, MATRIX_OUT_T>(V, V, Ir_V);

    // Print results
    printf("A = \n");
    hls::print_matrix<A_ROWS, A_COLS, MATRIX_IN_T, hls::NoTranspose>(A, "   ");

    printf("S = \n");
    hls::print_matrix<A_ROWS, A_COLS, MATRIX_OUT_T, hls::NoTranspose>(S, "   ");

    printf("U = \n");
    hls::print_matrix<A_ROWS, A_ROWS, MATRIX_OUT_T, hls::NoTranspose>(U, "   ");

    printf("V = \n");
    hls::print_matrix<A_COLS, A_COLS, MATRIX_OUT_T, hls::NoTranspose>(V, "   ");

    printf("A reconstructed = \n");
    hls::print_matrix<A_ROWS, A_COLS, MATRIX_OUT_T, hls::NoTranspose>(USV, "   ");

    printf("I reconstructed (U) = \n");
    hls::print_matrix<A_ROWS, A_ROWS, MATRIX_OUT_T, hls::NoTranspose>(Ir_U, "   ");

    printf("I reconstructed (V) = \n");
    hls::print_matrix<A_COLS, A_COLS, MATRIX_OUT_T, hls::NoTranspose>(Ir_V, "   ");

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
    double diff_ratio;
    diff_ratio = difference_ratio<A_ROWS, A_COLS>(USV,A);
    if ( diff_ratio > 50.0 ) {
      std::cout << "A reconstruct fails" << std::endl;
      fail = 1;
    }
    diff_ratio = difference_ratio<A_ROWS, A_COLS>(Ir_U,I_ref);
    if ( diff_ratio > 50.0 ) {
      std::cout << "I U fails" << std::endl;
      fail = 1;
    }
    diff_ratio = difference_ratio<A_ROWS, A_COLS>(Ir_V,I_ref);
    if ( diff_ratio > 50.0 ) {
      std::cout << "I V fails" << std::endl;
      fail = 1;
    }
    return(fail);
}

