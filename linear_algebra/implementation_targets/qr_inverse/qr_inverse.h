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

#ifndef QR_INVERSE_H
#define QR_INVERSE_H

#include "hls_linear_algebra.h"

const unsigned ROWS_COLS_A = 8;

// Define input/output types
typedef std::complex<float> MATRIX_IN_T;
typedef std::complex<float> MATRIX_OUT_T;

// Define default configuration class
// o Used as base class
// o Used to reference internal data types for sub-function configuration
typedef hls::qr_inverse_traits<ROWS_COLS_A, MATRIX_IN_T, MATRIX_OUT_T> DEFAULT_QR_INV_TRAITS;

// Define configuration class for each implementation "target"
struct QR_INV_CONFIG_SMALL : DEFAULT_QR_INV_TRAITS {
  struct QRF_CONFIG : hls::qrf_traits<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, DEFAULT_QR_INV_TRAITS::InternalType> {
    static const int CALC_ROT_II = 8;
    static const int UPDATE_II= 8;
  };
  struct BACK_SUB_CONFIG : hls::back_substitute_traits<ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,DEFAULT_QR_INV_TRAITS::InternalType> {
    static const int INNER_II = 8;
    static const int DIAG_II  = 8;
  };
  struct MULTIPLIER_CONFIG : hls::matrix_multiply_traits<hls::NoTranspose,hls::ConjugateTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,MATRIX_OUT_T> {
    static const int INNER_II = 4;
  };
};
struct QR_INV_CONFIG_BALANCED : DEFAULT_QR_INV_TRAITS {
  struct QRF_CONFIG : hls::qrf_traits<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, DEFAULT_QR_INV_TRAITS::InternalType> {
  };
  struct BACK_SUB_CONFIG : hls::back_substitute_traits<ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,DEFAULT_QR_INV_TRAITS::InternalType> {
    static const int INNER_II = 2;
    static const int DIAG_II  = 2;
  };
  struct MULTIPLIER_CONFIG : hls::matrix_multiply_traits<hls::NoTranspose,hls::ConjugateTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,MATRIX_OUT_T> {
    static const int INNER_II = 2;
  };
};
struct QR_INV_CONFIG_FAST : DEFAULT_QR_INV_TRAITS {
  struct QRF_CONFIG : hls::qrf_traits<ROWS_COLS_A, ROWS_COLS_A, MATRIX_IN_T, DEFAULT_QR_INV_TRAITS::InternalType> {
    static const int UPDATE_II= 2;
  };
  struct BACK_SUB_CONFIG : hls::back_substitute_traits<ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,DEFAULT_QR_INV_TRAITS::InternalType> {
  };
  struct MULTIPLIER_CONFIG : hls::matrix_multiply_traits<hls::NoTranspose,hls::ConjugateTranspose,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,ROWS_COLS_A,DEFAULT_QR_INV_TRAITS::InternalType,MATRIX_OUT_T> {
  };
};

// Declare top-level functions for each implementation "target"
void qr_inverse_small(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                      MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A],
                      int& inverse_OK);
void qr_inverse_balanced(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                         MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A],
                         int& inverse_OK);
void qr_inverse_default(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                        MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A],
                        int& inverse_OK);
void qr_inverse_fast(MATRIX_IN_T  A[ROWS_COLS_A][ROWS_COLS_A],
                     MATRIX_OUT_T InverseA[ROWS_COLS_A][ROWS_COLS_A],
                     int& inverse_OK);

#endif

