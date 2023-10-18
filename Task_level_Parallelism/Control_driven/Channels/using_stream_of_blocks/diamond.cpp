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

#include "diamond.h"

void diamond(hls::stream<data_t>& vecIn, hls::stream<data_t>& vecOut) {
#pragma HLS INTERFACE ap_ctrl_chain port = return

    hls::stream_of_blocks<block_data_t> c1, c2, c3, c4;

#pragma HLS dataflow
    funcA(vecIn, c1, c2);
    funcB(c1, c3);
    funcC(c2, c4);
    funcD(c3, c4, vecOut);
}

void funcA(hls::stream<data_t>& in, hls::stream_of_blocks<block_data_t>& out1,
           hls::stream_of_blocks<block_data_t>& out2) {
#pragma HLS INLINE off

funcA_Loop0:
    for (int i = 0; i < N / NUM_BLOCKS; i++) {
#pragma HLS pipeline II = 10

        // Obtain write locks for the two output channels
        hls::write_lock<block_data_t> out1L(out1);
        hls::write_lock<block_data_t> out2L(out2);

        // Read a block of 10 data items from the stream
    funcA_Loop1:
        for (unsigned int j = 0; j < NUM_BLOCKS; j++) {
            data_t t = in.read() * 3;
            out1L[j] = t;
            out2L[j] = t;
        }
    }
}

void funcB(hls::stream_of_blocks<block_data_t>& in,
           hls::stream_of_blocks<block_data_t>& out) {
#pragma HLS INLINE off

funcB_Loop0:
    for (int i = 0; i < N / NUM_BLOCKS; i++) {
#pragma HLS pipeline II = 5

        // Obtain a read lock on the input channel
        // and a write lock for the output channel
        hls::read_lock<block_data_t> inL(in);
        hls::write_lock<block_data_t> outL(out);
    funcB_Loop1:
        for (unsigned int j = 0; j < NUM_BLOCKS; j++)
            outL[j] = inL[j] + 25;
    }
}

void funcC(hls::stream_of_blocks<block_data_t>& in,
           hls::stream_of_blocks<block_data_t>& out) {
#pragma HLS INLINE off

Loop0:
    for (data_t i = 0; i < N / NUM_BLOCKS; i++) {
#pragma HLS pipeline II = 5

        // Obtain a read lock on the input channel
        // and a write lock for the output channel
        hls::read_lock<block_data_t> inL(in);
        hls::write_lock<block_data_t> outL(out);
        for (unsigned int j = 0; j < NUM_BLOCKS; ++j)
            outL[j] = inL[j] * 2;
    }
}

void funcD(hls::stream_of_blocks<block_data_t>& in1,
           hls::stream_of_blocks<block_data_t>& in2, hls::stream<data_t>& out) {
#pragma HLS INLINE off

Loop0:
    for (int i = 0; i < N / NUM_BLOCKS; i++) {
#pragma HLS pipeline II = 10

        // Obtain two read locks on the input channels
        hls::read_lock<block_data_t> in1L(in1);
        hls::read_lock<block_data_t> in2L(in2);
        for (unsigned int j = 0; j < NUM_BLOCKS; ++j)
            out.write(in1L[j] + in2L[j] * 2);
    }
}
