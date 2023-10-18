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
    A arr[N];
    for (unsigned i = 0; i < N; i++) {
        arr[i].foo[0] = i * 4;
        arr[i].foo[1] = i * 4 + 1;
        arr[i].foo[2] = i * 4 + 2;
        arr[i].bar = i * 4 + 3;
    }

    // 0: 0000_0003_0000_0002_0000_0001_0000_0000
    // 1: 0000_0007_0000_0006_0000_0005_0000_0004
    // 2: 0000_000b_0000_000a_0000_0009_0000_0008
    // 3: 0000_000f_0000_000e_0000_000d_0000_000c
    // 4: 0000_0013_0000_0012_0000_0011_0000_0010
    // 5: 0000_0017_0000_0016_0000_0015_0000_0014
    // 6: 0000_001b_0000_001a_0000_0019_0000_0018
    // 7: 0000_001f_0000_001e_0000_001d_0000_001c
    // 8: 0000_0023_0000_0022_0000_0021_0000_0020
    // 5: 0000_0027_0000_0026_0000_0025_0000_0024
    auto ret = dut(arr);
    std::cout << "ret = " << ret << std::endl;
    if (ret != 780)
        return 1;
    return 0;
}
