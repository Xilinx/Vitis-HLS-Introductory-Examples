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

void sub_task1(hls::stream<int>& in, hls::stream<int>& out) {
    int c = in.read();
    out.write(c + 2);
}

void sub_task2(hls::stream<int>& in, hls::stream<int>& out) {
    int c = in.read();
    out.write(c - 1);
}

void task2(hls::stream<int>& in, hls::stream<int>& out, int n) {
    int c = in.read();
    out.write(c + 2 + n);
}

void test(hls::stream<int>& in, hls::stream<int>& out, int n) {
#pragma HLS STABLE variable = n
    HLS_TASK_STREAM<int> s1;
    HLS_TASK_STREAM<int> s2;
    HLS_TASK t(task2, s2, out, n);
    HLS_TASK t1(sub_task1, in, s1);
    HLS_TASK t2(sub_task2, s1, s2);
}

