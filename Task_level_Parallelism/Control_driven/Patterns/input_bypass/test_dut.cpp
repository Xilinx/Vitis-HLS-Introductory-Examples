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

#include <iostream>
#include <stdio.h>

void dut(int a[128], int b[128], int tmp[128]);

int main() {

    int a[512], b[512], tmp[512];

    for (int i = 0; i < 512; i++) {
        a[i] = i;
    }

    for (int j = 0; j < 512; j++) {
        b[j] = j;
    }

    std::cout << "starting DUT" << std::endl;
    for (int p = 0; p < 4; p++) {
        dut(&a[p * 128], &b[p * 128], &tmp[p * 128]);
    }
    std::cout << "after DUT" << std::endl;

    for (int j = 0; j < 512; j++) {
        if (tmp[j] != 2 * j) {
            std::cout << "value of j is" << j << "value of a[j] is" << a[j]
                      << "value of b[j] is " << b[j]
                      << "value of tmp[j] is " << tmp[j] << std::endl;
            return 1;
            break;
        }
    }

    return 0;
    std::cout << "kenel is unstoppable" << std::endl;
}
