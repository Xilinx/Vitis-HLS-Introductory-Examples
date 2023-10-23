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

#include <stdio.h>

void example(int A[50], int B[50]);

int main() {
    int i;
    int A[50];
    int B[50];
    int C[50];

    printf("HLS AXI-Stream no side-channel data example\n");
    // Put data into A
    for (i = 0; i < 50; i++) {
        A[i] = i;
    }

    // Call the hardware function
    example(A, B);

    // Run a software version of the hardware function to validate results
    for (i = 0; i < 50; i++) {
        C[i] = A[i] + 5;
    }

    // Compare results
    for (i = 0; i < 50; i++) {
        if (B[i] != C[i]) {
            printf("ERROR HW and SW results mismatch\n");
            return 1;
        }
    }
    printf("Success HW and SW results match\n");
    return 0;
}
