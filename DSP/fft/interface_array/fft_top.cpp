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

void fft_top(ap_uint<1> dir,
             ap_uint<15> scaling,
             cmpxDataIn xn[FFT_LENGTH],
             cmpxDataOut xk[FFT_LENGTH],
             bool* status)
{
#pragma HLS interface ap_fifo depth=1 port=status
#pragma HLS interface ap_fifo depth=1024 port=xn,xk
#pragma HLS stream variable=xn
#pragma HLS stream variable=xk
#pragma HLS dataflow

    hls::fft<config1>(xn, xk, dir, scaling, -1, status);

}

