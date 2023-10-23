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
#include "test.h"

int main() {
    hls::stream<int> a;
    hls::stream<int> b1;
    hls::stream<int> b2;
    hls::stream<int> b3;
    hls::stream<int> b4;
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    int cnt4 = 0;

    for (int i = 0; i < 16; i++) {
        b1.write(i);
        b2.write(i);
        b3.write(i);
        b4.write(i);
    }

    dut(a, b1, b2, b3, b4);

    int sum = 0;
    for (int i = 0; i < 64; i++) {
        int tmp1 = a.read();
        sum += tmp1;
    }

    if (sum != 480)
        return 1;

    return 0;
}
