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

void transfer_kernel(hls::burst_maxi<int> in, hls::burst_maxi<int> out,
                     const int size) {
#pragma HLS INTERFACE m_axi port = in depth = 512 latency = 32 offset =        \
    slave bundle = bundle1
#pragma HLS INTERFACE m_axi port = out depth = 5120 offset = slave latency =   \
    32 bundle = bundle2

    int buf[8192];
    in.read_request(0, size);
    for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE II = 1
        buf[i] = in.read();
    }

    out.write_request(0, size * NUM);

    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < size; j++) {
#pragma HLS PIPELINE II = 1
            int a = buf[j];
            out.write(a);
        }
    }
    out.write_response();
}
