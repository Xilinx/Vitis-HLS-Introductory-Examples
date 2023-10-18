/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
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

#include "ap_fixed.h"
#include "hls_fft.h"

// configurable params
const char FFT_INPUT_WIDTH = 16;
const char FFT_OUTPUT_WIDTH = FFT_INPUT_WIDTH;
const char FFT_CONFIG_WIDTH = 16;
const char FFT_NFFT_MAX = 10;
const int FFT_LENGTH = 1 << FFT_NFFT_MAX;

#include <complex>
using namespace std;

struct config1 : hls::ip_fft::params_t {
    static const unsigned ordering_opt = hls::ip_fft::natural_order;
    static const unsigned config_width = FFT_CONFIG_WIDTH;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;

typedef ap_fixed<FFT_INPUT_WIDTH, 1> data_in_t;
typedef ap_fixed<FFT_OUTPUT_WIDTH, FFT_OUTPUT_WIDTH - FFT_INPUT_WIDTH + 1>
    data_out_t;
typedef std::complex<data_in_t> cmpxDataIn;
typedef std::complex<data_out_t> cmpxDataOut;

void dummy_proc_fe(bool direction, config_t* config, cmpxDataIn in[FFT_LENGTH],
                   cmpxDataIn out[FFT_LENGTH]);

void dummy_proc_be(status_t* status_in, bool* ovflo, cmpxDataOut in[FFT_LENGTH],
                   cmpxDataOut out[FFT_LENGTH]);

void fft_top(bool direction, cmpxDataIn in[FFT_LENGTH],
             cmpxDataOut out[FFT_LENGTH], bool* ovflo);
