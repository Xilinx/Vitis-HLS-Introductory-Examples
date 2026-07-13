/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "array_partition_lib_simple.h"

// Sums 4 strided elements of A: A[i], A[i+s], A[i+2s], A[i+3s].
// The stride s is a runtime value, so HLS cannot statically prove that the
// 4 parallel accesses land in different banks -- #pragma HLS ARRAY_PARTITION
// alone cannot deliver II=1 here. hls::gather<T,K> shifts that guarantee to
// the caller (see main's test cases: s must be coprime with K=4) and reaches
// II=1 regardless of the index pattern.
float array_partition_lib_simple(float A[ARRAY_SIZE], size_t i, size_t s) {
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=K
    auto v = hls::gather<float, K>(A, {i + s*0, i + s*1, i + s*2, i + s*3});
    return v.reduce_add();
}
