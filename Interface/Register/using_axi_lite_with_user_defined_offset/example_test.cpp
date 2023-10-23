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

void example(char* a, char* b, char* c);

int main() {

    char a;
    char b;
    char c;
    char d;
    char sw_result;

    printf("HLS AXI-Lite Example\n");
    printf("Function c += a + b\n");
    printf("Initial values a = 5, b = 10, c = 0\n");

    a = 5;
    b = 10;
    c = 0;
    d = 0;

    example(&a, &b, &c);
    d += a + b;

    printf("HW result = %d\n", c);
    printf("SW result = %d\n", d);

    if (d == c) {
        printf("Success SW and HW results match\n");
        return 0;
    } else {
        printf("ERROR SW and HW results mismatch\n");
        return 1;
    }
}
