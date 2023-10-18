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

void producer1(hls::stream<int>& out, hls::stream<int>& in) {
#pragma HLS inline off
    for (int i = 0; i < 16; i++) {
        int tmp_d = in.read();
        out.write(tmp_d);
    }
}

void producer2(hls::stream<int>& out, hls::stream<int>& in) {
#pragma HLS inline off
    for (int i = 0; i < 16; i++) {
        int tmp_d = in.read();
        out.write(tmp_d);
    }
}

void producer3(hls::stream<int>& out, hls::stream<int>& in) {
#pragma HLS inline off
    for (int i = 0; i < 16; i++) {
        int tmp_d = in.read();
        out.write(tmp_d);
    }
}

void producer4(hls::stream<int>& out, hls::stream<int>& in) {
#pragma HLS inline off
    for (int i = 0; i < 16; i++) {
        int tmp_d = in.read();
        out.write(tmp_d);
    }
}

void consumer1(hls::stream<int>& in, hls::stream<int>& out) {
#pragma HLS inline off
    for (int i = 0; i < 64; i++) {
        int tmp = in.read();
        out.write(tmp);
    }
}

void dut(hls::stream<int>& in1, hls::stream<int>& out1, hls::stream<int>& out2,
         hls::stream<int>& out3, hls::stream<int>& out4) {
#pragma HLS dataflow

    hls::merge::round_robin<int, 4, 5, 100> s;

    producer1(s.in[0], out1);
    producer2(s.in[1], out2);
    producer3(s.in[2], out3);
    producer4(s.in[3], out4);
    consumer1(s.out, in1);
}
