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

extern "C" void dut(const double* in, // Read-Only Vector 1
                    double* out,      // Output Result
                    int size          // Size in integer
);

void dut_sw(const double* in, // Read-Only Vector 1
            double* out,      // Output Result
            int size          // Size in integer
) {
    for (int i = 0; i < size; i++) {
        out[i] = in[i] + in[i + 1];
    }
}

int main() {
    double A[N + 1];
    double B[N + 1], C[N + 1];
    const double* in = A;
    double* out = B;
    double* ref = C;
    int i = 0;

    for (i = 0; i < N + 1; i++) {
        A[i] = i;
        B[i] = 0;
        C[i] = 0;
    }

    dut(in, out, N);
    dut_sw(in, ref, N);

    int err = 0;

    for (i = 0; i < N; i++)
        if (fabs(out[i] / ref[i] - 1) > 1e-6) {
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
