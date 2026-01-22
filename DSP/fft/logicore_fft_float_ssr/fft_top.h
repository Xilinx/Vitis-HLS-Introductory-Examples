/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "hls_fft.h"

// User configurable: Choose interface type
// Set to 1 for STREAM interface, 0 for ARRAY interface
#ifndef USE_STREAM_INTERFACE
#define USE_STREAM_INTERFACE 1
#endif

// Example configurable params
#define FFT_SSR 2 // Super Sample Rate
#define FFT_NFFT_MAX 10 // 1024-point FFT

const char FFT_INPUT_WIDTH = 32;
const char FFT_OUTPUT_WIDTH = FFT_INPUT_WIDTH;
const int FFT_LENGTH = 1 << FFT_NFFT_MAX;

#include <complex>
using namespace std;

// FFT configuration struct, inheriting from hls::ip_fft::ssr_params_t
// Here we set SSR and specify floating point
struct config1 : hls::ip_fft::ssr_params_t {
    static const unsigned super_sample_rate = FFT_SSR;
    static const bool use_native_float = true;
    static const bool systolicfft_inv = false;
    static const unsigned max_nfft = FFT_NFFT_MAX;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;

typedef float data_in_t;
typedef float data_out_t;

typedef std::complex<data_in_t> cmpxDataIn;
typedef std::complex<data_out_t> cmpxDataOut;

// Function declaration with interface determined by macro
#if USE_STREAM_INTERFACE
void fft_top(bool direction,
             hls::stream<hls::vector<cmpxDataIn, FFT_SSR>> &in,
             hls::stream<hls::vector<cmpxDataOut, FFT_SSR>> &out,
             bool* status);
#else
void fft_top(bool direction, cmpxDataIn in[FFT_LENGTH],
             cmpxDataOut out[FFT_LENGTH], bool* status);
#endif
