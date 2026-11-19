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
#include "fft_top.h"

void fft_top(hls::stream<input_t> &xn,
             hls::stream<output_t> &xk)
{
#pragma HLS dataflow

    // The FFT call is the only process of the design. Each call transforms
    // one frame of FFT_LENGTH / SAMPLE_RATE input words.
    hls::vrf_fft::fft<config>(xn, xk);

}
