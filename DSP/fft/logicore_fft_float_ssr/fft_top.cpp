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
#include "fft_top.h"

#if USE_STREAM_INTERFACE
// Stream interface implementation
void fft_top(bool direction,
             hls::stream<hls::vector<cmpxDataIn, FFT_SSR>> &in,
             hls::stream<hls::vector<cmpxDataOut, FFT_SSR>> &out,
             bool* status)
{
#pragma HLS interface ap_hs port = direction
#pragma HLS interface ap_fifo depth = 1 port = status
#pragma HLS dataflow

    // no direction and no overflow for SSR>1
    hls::fft<config1>(in, out);
}

#else
// Array interface implementation
void fft_top(bool direction, std::complex<data_in_t> in[FFT_LENGTH],
             std::complex<data_out_t> out[FFT_LENGTH], bool* status) {
#pragma HLS interface ap_hs port = direction
#pragma HLS interface ap_fifo depth = 1 port = status
#pragma HLS dataflow

    // no direction parameter for SSR>1 array interface
    hls::fft<config1>(in, out);
}
#endif

