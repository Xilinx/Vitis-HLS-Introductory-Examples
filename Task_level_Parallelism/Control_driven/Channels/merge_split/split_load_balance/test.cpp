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

void producer(hls::stream<ap_uint<32>>& out, hls::stream<int>& in) {
#pragma HLS inline off
    ap_uint<32> tmp;
    for (int i = 0; i < 16; i++) {
        int tmp_d = in.read();
        out.write(tmp_d);
    }
}

void consumer1(hls::stream<ap_uint<32>>& in, hls::stream<ap_uint<32>>& out) {
#pragma HLS inline off
    for (int i = 0; i < 4; i++) {
        ap_uint<32> tmp = in.read();
        out.write(tmp);
    }
}

void consumer2(hls::stream<ap_uint<32>>& in, hls::stream<ap_uint<32>>& out) {
#pragma HLS inline off
    for (int i = 0; i < 4; i++) {
        ap_uint<32> tmp = in.read();
        out.write(tmp);
    }
}

void consumer3(hls::stream<ap_uint<32>>& in, hls::stream<ap_uint<32>>& out) {
#pragma HLS inline off
    for (int i = 0; i < 4; i++) {
        ap_uint<32> tmp = in.read();
        out.write(tmp);
    }
}

void consumer4(hls::stream<ap_uint<32>>& in, hls::stream<ap_uint<32>>& out) {
#pragma HLS inline off
    for (int i = 0; i < 4; i++) {
        ap_uint<32> tmp = in.read();
        out.write(tmp);
    }
}

void dut(hls::stream<int>& in1, hls::stream<ap_uint<32>>& out1,
         hls::stream<ap_uint<32>>& out2, hls::stream<ap_uint<32>>& out3,
         hls::stream<ap_uint<32>>& out4) {
#pragma HLS dataflow

    hls::split::load_balance<ap_uint<32>, 4, 6> s;

    producer(s.in, in1);
    consumer1(s.out[0], out1);
    consumer2(s.out[1], out2);
    consumer3(s.out[2], out3);
    consumer4(s.out[3], out4);
}
