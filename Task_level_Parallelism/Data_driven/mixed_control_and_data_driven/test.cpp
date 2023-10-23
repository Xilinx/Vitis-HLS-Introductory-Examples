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
#include "test.h"

void worker(hls::stream<int>& in, hls::stream<int>& out) {
    int i = in.read();
    int o = i * 2 + 1;
    out.write(o);
}

void read_in(int* in, int n, hls::stream<int>& out) {
    for (int i = 0; i < n; i++) {
        out.write(in[i]);
    }
}

void write_out(hls::stream<int>& in, int* out, int n) {
    for (int i = 0; i < n; i++) {
        out[i] = in.read();
    }
}

void dut(int in[N], int out[N], int n) {
    hls_thread_local hls::split::round_robin<int, NP> split1;
    hls_thread_local hls::merge::round_robin<int, NP> merge1;
#pragma HLS dataflow

    read_in(in, n, split1.in);

    // Task-Channels
    hls_thread_local hls::task t[NP];
    for (int i = 0; i < NP; i++) {
#pragma HLS unroll
        t[i](worker, split1.out[i], merge1.in[i]);
    }

    write_out(merge1.out, out, n);
}
