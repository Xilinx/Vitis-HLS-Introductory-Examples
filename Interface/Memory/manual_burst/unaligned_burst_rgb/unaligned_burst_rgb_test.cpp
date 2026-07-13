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
#include "unaligned_burst_rgb.h"
#include <iostream>

rgb_t A[MAX_ELEMENTS];
rgb_t B[MAX_ELEMENTS];

int main(void) {
    int n_elements = 100; // Number of rgb_t elements to process (must be <= MAX_ELEMENTS)

    for (unsigned i = 0; i < MAX_ELEMENTS; i++) {
        A[i] = rgb_t({(char)(i % 255), (char)(i % 255 + 1), (char)(i % 255 + 2)});
    }

    std::cout << "(i) Starting HLS simulation... with " << n_elements << " elements" << std::endl;
    unaligned_burst_rgb((ap_uint<512>*)A, (ap_uint<512>*)B, n_elements);

    for (int i = 0; i < n_elements; i++) {
        if (A[i].R != B[i].R || A[i].G != B[i].G || A[i].B != B[i].B) {
            std::cout << "(!) Error at index " << i
                       << ": expected (" << (int)A[i].R << ", " << (int)A[i].G << ", " << (int)A[i].B
                       << ") but got (" << (int)B[i].R << ", " << (int)B[i].G << ", " << (int)B[i].B << ")"
                       << std::endl;
            return EXIT_FAILURE;
        }
    }
    std::cout << "(i) Test passed!" << std::endl;
    return EXIT_SUCCESS;
}
