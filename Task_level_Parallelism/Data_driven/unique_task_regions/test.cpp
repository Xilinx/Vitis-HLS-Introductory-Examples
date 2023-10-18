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

void copy(hls::stream<int>& in, hls::stream<int>& out) {
    for (int i = 0; i < 8; i++)
        out.write(in.read());
}

void func(hls::stream<int>& in, hls::stream<int>& out) {
    int cnt = 0;
    // This loop is only needed for the persistent variable above
    while (1) {
#pragma HLS pipeline off
        int t = in.read();
        out.write(t + cnt++);
    }
}

template <typename>
void mid(hls::stream<int>& in1, hls::stream<int>& out1, hls::stream<int>& in2,
         hls::stream<int>& out2) {
#pragma HLS dataflow
    HLS_TASK_STREAM<int> in1_t, out1_t, in2_t, out2_t;
    HLS_TASK t1(func, in1_t, out1_t);
    HLS_TASK t2(func, in2_t, out2_t);

    copy(in1, in1_t);
    copy(in2, in2_t);
    copy(out1_t, out1);
    copy(out2_t, out2);
}

void middle(hls::stream<int>& in1, hls::stream<int>& out1,
            hls::stream<int>& in2, hls::stream<int>& out2,
            hls::stream<int>& in3, hls::stream<int>& out3,
            hls::stream<int>& in4, hls::stream<int>& out4) {
    /* Top level dataflow region that calls a task region uniquely via template
     * instantiation */
#pragma HLS dataflow
    mid<class c1>(in1, out1, in2, out2);
    mid<class c2>(in3, out3, in4, out4);
}

/* ************************************************************************ */
/* ************* TOP LEVEL FUNCTION *************************************** */
/* ************************************************************************ */
void top(hls::stream<int>& in1, hls::stream<int>& out1, hls::stream<int>& in2,
         hls::stream<int>& out2, hls::stream<int>& in3, hls::stream<int>& out3,
         hls::stream<int>& in4, hls::stream<int>& out4) {
    /* two calls to the lower level dataflow region */
    middle(in1, out1, in2, out2, in3, out3, in4, out4);
    middle(in1, out1, in2, out2, in3, out3, in4, out4);
}
