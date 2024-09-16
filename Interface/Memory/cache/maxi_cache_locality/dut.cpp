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
void dut(const double* in, // Read-Only Vector 1
         double* out,      // Output Result
         int size          // Size in integer
) {

#pragma HLS INTERFACE mode = m_axi bundle = aximm2 depth =                     \
    1025 max_read_burst_length = 128 num_read_outstanding = 256 port = in
#pragma HLS INTERFACE m_axi port = out bundle = aximm depth = 1024

#pragma HLS cache port = in lines = 1 depth = 128

    for (int i = 0; i < size; i++) {
        out[i] = in[i] + in[i + 1];
    }
}
}
