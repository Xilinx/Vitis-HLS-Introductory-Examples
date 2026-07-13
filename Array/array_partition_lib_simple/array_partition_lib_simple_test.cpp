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

#include <cstddef>
#include <iostream>

int main(void) {
    float A[ARRAY_SIZE];

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        A[i] = (float)(i + 1);
    }

    size_t errors = 0;

    // Stride must be coprime with K=4 (i.e. odd) to guarantee conflict-free
    // banks for hls::gather's cyclic partitioning.
    struct { size_t i; size_t s; } tests[] = {
        {0, 1},   // consecutive: A[0]+A[1]+A[2]+A[3]
        {4, 1},   // consecutive: A[4]+A[5]+A[6]+A[7]
        {2, 3},   // stride 3:    A[2]+A[5]+A[8]+A[11]
        {0, 5},   // stride 5:    A[0]+A[5]+A[10]+A[15]
        {1, 9},   // stride 9:    A[1]+A[10]+A[19]+A[28]
    };

    for (auto &t : tests) {
        float expected = A[t.i] + A[t.i + t.s] + A[t.i + 2*t.s] + A[t.i + 3*t.s];
        float result = array_partition_lib_simple(A, t.i, t.s);
        if (result != expected) {
            std::cout << "(!) Error: array_partition_lib_simple(A, " << t.i << ", " << t.s << ") = "
                       << result << " (expected: " << expected << ")" << std::endl;
            ++errors;
        }
    }

    if (errors > 0) {
        std::cout << "FAIL: " << errors << " error(s)." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "(i) Test passed!" << std::endl;
    return EXIT_SUCCESS;
}
