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
 Static configuration of the Versal RF FFT hard IP (hls_vrf_fft.h)
    SAMPLE_RATE         Samples per clock: sr_1, sr_2, sr_4 or sr_8
    FFT_MAX_POINT_SIZE  log2 of the maximum transform size: 3 to 12 (2^12 = 4096 points)
    FFT_SCALING         true: scaled output, the scaling factor is set for each frame
                        false: full precision output (DATA_WIDTH_OUT must be 31)
    FFT_REORDER         false: bit-reversed output order, true: natural order
                        (natural order requires FFT_SCALING and FFT_MAX_POINT_SIZE <= 11)
    DATA_WIDTH_IN       Input data width: 16 or 18 bits
    DATA_WIDTH_OUT      Output data width: 16, 18 (scaled) or 31 (unscaled) bits
    ACTIVE_DUTY_CYCLE,  Inputs of the power estimate only (0 to 100 percent)
    TOGGLE_RATE
****************************************************************/
struct config : hls::vrf_fft::params_t {
    static const unsigned SAMPLE_RATE         = hls::vrf_fft::sr_1;
    static const unsigned FFT_MAX_POINT_SIZE  = 11;    // 2^11 = 2048 points
    static const bool     FFT_SCALING         = true;
    static const bool     FFT_REORDER         = false; // bit-reversed output: use getIndex()
    static const unsigned DATA_WIDTH_IN       = 18;
    static const unsigned DATA_WIDTH_OUT      = 18;
    static const unsigned ACTIVE_DUTY_CYCLE   = 25;
    static const unsigned TOGGLE_RATE         = 25;
};

typedef hls::vrf_fft::input_t<config>  input_t;
typedef hls::vrf_fft::output_t<config> output_t;

// Transform size used by this example
const int FFT_NFFT   = config::FFT_MAX_POINT_SIZE; // log2 of the number of points
const int FFT_LENGTH = 1 << FFT_NFFT;              // 2048 points

void fft_top(hls::stream<input_t> &xn,
             hls::stream<output_t> &xk);
