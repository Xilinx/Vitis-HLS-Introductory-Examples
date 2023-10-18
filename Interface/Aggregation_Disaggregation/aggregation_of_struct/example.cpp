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

int dut(A arr[N]) {
#pragma HLS interface ap_fifo port = arr
#pragma HLS aggregate variable = arr
    int sum = 0;
    for (unsigned i = 0; i < N; i++) {
        auto tmp = arr[i];
        sum += tmp.foo[0] + tmp.foo[1] + tmp.foo[2] + tmp.bar;
    }
    return sum;
}
