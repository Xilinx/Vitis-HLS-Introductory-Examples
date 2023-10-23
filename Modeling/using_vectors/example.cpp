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
#include "example.h"
#include <iostream>

template <int N, typename T> void load(T (&out)[N], const T* in) {
#pragma HLS INLINE off
    for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
        out[i] = in[i];
    }
}

template <int N, typename T> void store(T* out, const T (&in)[N]) {
#pragma HLS INLINE off
    for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
        out[i] = in[i];
    }
}

template <int N, typename T, typename S>
void compute(T (&res)[N], const S (&lhs)[N], const S (&rhs)[N]) {
#pragma HLS INLINE off
    for (int i = 0; i < N; ++i) {
#pragma HLS pipeline
        res[i] = lhs[i] + rhs[i];
    }
}

extern "C" void example(float16* res, const float16* lhs, const float16* rhs,
                        int n) {
#pragma HLS INTERFACE m_axi port = lhs offset = slave bundle = gmem0 depth = 32
#pragma HLS INTERFACE m_axi port = rhs offset = slave bundle = gmem1 depth = 32
#pragma HLS INTERFACE m_axi port = res offset = slave bundle = gmem0 depth = 32

    for (int i = 0; i < n; ++i) {
        float16 lhs_buf[NUM];
        float16 rhs_buf[NUM];
        float16 res_buf[NUM];

#pragma HLS DATAFLOW
        load(lhs_buf, lhs);
        load(rhs_buf, rhs);
        compute(res_buf, lhs_buf, rhs_buf);
        store(res, res_buf);
    }
}
