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

#include "free_pipe_mult.h"
#include <iostream>

int accumulate(data_t A[]) {
#pragma HLS inline off

    data_t acc = 0.0;
    for (int i = 0; i < SZ; i++) {
        std::cout << "A: " << A[i] << std::endl;
        acc += A[i];
    }
    return acc;
}

void process(hls::stream<data_t>& strm_in, hls::stream<data_t>& strm_out) {
#pragma HLS inline off

    for (int i = 0; i < SZ; i++) {
        data_t tmp;
        tmp = strm_in.read();
        strm_out.write(tmp);
    }
}

void inner(data_t A[SZ], hls::stream<data_t>& stream_in, data_t* out) {
#pragma HLS pipeline

#pragma HLS INTERFACE ap_fifo port = stream_in
    data_t regA[SZ];
#pragma HLS ARRAY_PARTITION variable = regA complete
    for (int i = 0; i < SZ; i++) {
        data_t tmp;
        tmp = stream_in.read();
        regA[i] = A[i] + tmp;
    }

    *out = accumulate(regA);
}

void free_pipe_mult(data_t A[SZ], hls::stream<data_t>& strm, data_t* out) {
#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_fifo port = strm

    data_t B[SZ];

    for (int i = 0; i < SZ; i++)
        B[i] = A[i] + i;

    hls::stream<data_t> strm_out;
    process(strm, strm_out);
    inner(B, strm_out, out);
}
