/*
 * Copyright 2026 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "hls_vrf_fft.h"

/***************************************************************
 Two Versal RF FFT hard IP instances with different configurations
 (see the base example for the description of each parameter):
   config_fwd: forward FFT, scaled, natural order output, 16-bit input and 18-bit output
   config_inv: inverse FFT, unscaled (full precision), 18-bit input and 31-bit output
 The output of the forward FFT in natural order is the input of the inverse FFT.
 Unscaled output requires DATA_WIDTH_OUT = 31 and cannot be reordered, so the
 output of the inverse FFT is in bit-reversed order.
****************************************************************/
struct config_fwd : hls::vrf_fft::params_t {
    static const unsigned SAMPLE_RATE         = hls::vrf_fft::sr_1;
    static const unsigned FFT_MAX_POINT_SIZE  = 5;     // 2^5 = 32 points
    static const bool     FFT_SCALING         = true;
    static const bool     FFT_REORDER         = true;  // natural order output
    static const unsigned DATA_WIDTH_IN       = 16;
    static const unsigned DATA_WIDTH_OUT      = 18;
    static const unsigned ACTIVE_DUTY_CYCLE   = 25;
    static const unsigned TOGGLE_RATE         = 25;
};

struct config_inv : hls::vrf_fft::params_t {
    static const unsigned SAMPLE_RATE         = hls::vrf_fft::sr_1;
    static const unsigned FFT_MAX_POINT_SIZE  = 5;     // 2^5 = 32 points
    static const bool     FFT_SCALING         = false; // full precision output
    static const bool     FFT_REORDER         = false; // bit-reversed output: use getIndex()
    static const unsigned DATA_WIDTH_IN       = 18;
    static const unsigned DATA_WIDTH_OUT      = 31;
    static const unsigned ACTIVE_DUTY_CYCLE   = 25;
    static const unsigned TOGGLE_RATE         = 25;
};

typedef hls::vrf_fft::input_t<config_fwd>  fwd_in_t;
typedef hls::vrf_fft::output_t<config_fwd> fwd_out_t;
typedef hls::vrf_fft::input_t<config_inv>  inv_in_t;
typedef hls::vrf_fft::output_t<config_inv> inv_out_t;

// Types of the top-level function
typedef fwd_in_t  input_t;
typedef inv_out_t output_t;

// Transform size used by this example
const int FFT_NFFT   = config_fwd::FFT_MAX_POINT_SIZE; // log2 of the number of points
const int FFT_LENGTH = 1 << FFT_NFFT;                  // 32 points

void fft_top(hls::stream<input_t> &xn,
             hls::stream<output_t> &xk);
