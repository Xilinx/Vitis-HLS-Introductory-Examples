/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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

#include <iostream>
#include <math.h>
#include <string.h>

#define N 1024

extern "C" void dut(const unsigned int* in, // Read-Only Vector 1
                    unsigned int* out,      // Output Result
                    int size                // Size in integer
);

void dut_sw(const unsigned int* in, // Read-Only Vector 1
            unsigned int* out,      // Output Result
            int size                // Size in integer
) {
    for (int i = 0; i < size; i++) {
        if (i != 512)
            out[i] = in[i] + 1;
        else
            out[i] = in[i];
    }
}

int main() {
    unsigned int A[N];
    unsigned int B[N], C[N];
    const unsigned int* in = A;
    unsigned int* out = B;
    unsigned int* ref = C;
    int i = 0;

    for (i = 0; i < N; i++)
        A[i] = i;

    for (i = 0; i < N; i++) {
        B[i] = 0;
        C[i] = 0;
    }

    dut(in, out, N);
    dut_sw(in, ref, N);

    int err = 0;

    for (i = 0; i < N; i++)
        if (ref[i] != out[i]) {
            std::cout << "index " << i << " ref = " << ref[i]
                      << " out = " << out[i] << std::endl;
            err++;
        }

    if (err) {
        std::cout << "FAIL" << std::endl;
        return -1;
    } else
        std::cout << "PASS" << std::endl;

    return 0;
}
