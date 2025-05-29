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

    void* axim = malloc(SIZE*sizeof(unsigned));
    A_t* aximA = (A_t*)axim;
    B_t* aximB = (B_t*)axim;
    hls::stream<A_t> in;
    hls::stream<B_t> out;

    // Create input stream data
    for (unsigned i=0; i<SIZE; i+=RATIO) {
        A_t temp; temp.data = 0;
        for (int j=RATIO-1; j>=0; j--) {
            A_t shift; shift.data = i+j;
            temp.data |= shift.data<<(sizeof(B_t)*8*j);
        }
        in << temp;
    }

    // Instantiate the top level function
    dut(in, aximA, aximB, out);

    // Check output stream data
    unsigned errors = 0;
    for (unsigned i=0; i<DEPTHB; i++) {
        unsigned temp = unsigned(out.read().data);
        if (temp != i) {
            std::cout << "ERROR: E " << i << ", A " << temp << std::endl;
            errors++;
        }
    }

    free(axim);
    if (errors) {
        std::cout << "FAILURE" << std::endl;
        return 1;
    }
    else {
        std::cout << "SUCCESS" << std::endl;
        return 0;
    }
}
