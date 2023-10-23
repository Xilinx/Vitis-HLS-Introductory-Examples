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

void splitter(hls::stream<int>& in, hls::stream<int>& odds_buf,
              hls::stream<int>& evens_buf) {
    int data = in.read();
    if (data % 2 == 0)
        evens_buf.write(data);
    else
        odds_buf.write(data);
}

void odds(hls::stream<int>& in, hls::stream<int>& out) {
    out.write(in.read() + 1);
}

void evens(hls::stream<int>& in, hls::stream<int>& out) {
    out.write(in.read() + 2);
}

void odds_and_evens(hls::stream<int>& in, hls::stream<int>& out1,
                    hls::stream<int>& out2) {
    hls_thread_local hls::stream<int, N / 2> s1; // channel connecting t1 and t2
    hls_thread_local hls::stream<int, N / 2> s2; // channel connecting t1 and t3

    // t1 infinitely runs func1, with input in and outputs s1 and s2
    hls_thread_local hls::task t1(splitter, in, s1, s2);

    // t2 infinitely runs func2, with input s1 and output out1
    hls_thread_local hls::task t2(odds, s1, out1);

    // t3 infinitely runs func3, with input s2 and output out2
    hls_thread_local hls::task t3(evens, s2, out2);
}
