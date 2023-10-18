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

int main() {
    A a_in[NUM];
    A a_out[NUM];

    for (unsigned i = 0; i < NUM; i++) {
        a_in[i].s_1 = i;
        a_in[i].s_2 = i + 1;
        a_in[i].s_3 = i + 2;
        a_in[i].s_4 = i + 3;
        a_in[i].s_5 = i + 4;
        a_in[i].s_6 = i + 5;
    }

    int size = 8;

    dut(a_in, a_out, size);

    for (unsigned int i = 0; i < NUM; i++) {
        if (a_out[i].s_1 != a_in[i].s_1 + size) {
            return -1;
        }
    }
    printf("Test Pass!\n");
    return 0;
}
