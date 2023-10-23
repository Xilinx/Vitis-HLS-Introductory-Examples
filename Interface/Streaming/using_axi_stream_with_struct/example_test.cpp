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
#include <stdio.h>

int main() {
    short int i;
    mystream A, B, C;

    printf("HLS AXI-Stream no side-channel data example\n");

    // Put data into A Stream
    for (i = 0; i < SIZE; i++) {
        data_t tmp;
        tmp.data = {i, i};
        tmp.last = (i == (SIZE - 1)) ? 1 : 0;
        A.write(tmp);
    }

    // Call the hardware function
    example(A, B);

    // Run a software version of the hardware function to validate results
    for (i = 0; i < SIZE; i++) {
        data_t tmp_c;
        tmp_c.data.real(i + 5);
        tmp_c.data.imag(i + 1);
        C.write(tmp_c);
    }

    // Compare the results
    for (i = 0; i < SIZE; i++) {
        data_t tmp_b = B.read();
        data_t tmp_c = C.read();
        if (tmp_b.data != tmp_c.data) {
            printf("ERROR HW and SW results mismatch\n");
            return 1;
        }
    }
    printf("Success HW and SW results match\n");
    return 0;
}
