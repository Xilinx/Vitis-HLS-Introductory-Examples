/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include <iostream>

int main()
{

    int A[N][N];
    hls::stream<int> in;
    hls::stream<int> out;

    // Create input stream data
    for (unsigned int i=0; i < N; ++i) {
        for (unsigned int j=0; j < N; ++j) {
            std::cout << "TB: Writing (" << i << "," << j << ") = " << i+j << std::endl;
            in << i+j;
        }
    }

    // Instantiate the top level function
    dut(in, A, out);

    // Check output stream data
    unsigned errors = 0;
    for (unsigned i=0; i < N; ++i) {
        for (unsigned j=0; j < N; ++j) {
            unsigned temp = out.read();
            if (temp != i+j) {
                std::cout << "ERROR: A[" << i << "][" << j << "] does not match" << std::endl;
                errors++;
            }
        }
    }

    if (errors) {
        std::cout << "FAILURE" << std::endl;
        return 1;
    }
    else {
        std::cout << "SUCCESS" << std::endl;
        return 0;
    }
}
