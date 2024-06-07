/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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

extern "C" {
void dut(const unsigned int* in, // Read-Only Vector 1
         unsigned int* out,      // Output Result
         int size                // Size in integer
) {

#pragma HLS INTERFACE m_axi port = in bundle = aximm depth = 1024
#pragma HLS INTERFACE m_axi port = out bundle = aximm depth = 1024

// new cache pragma
#pragma HLS cache port = in lines = 8 depth = 128

    for (int i = 0; i < size; i++) {
#pragma HLS pipeline II = 1
        int t;
        if (i != 512)
            t = in[i] + 1;
        else
            t = in[i];
        out[i] = t;
    }
}
}
