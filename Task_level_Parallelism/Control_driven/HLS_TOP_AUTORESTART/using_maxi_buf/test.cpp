/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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

void my_dut::top(hls::m_axi_buf<int, hls::ap_none<size_t>> buf,
                 hls::stream<int> &out) {
#pragma HLS interface s_axilite autorestart port=return
#pragma HLS interface m_axi     port=buf.base offset=off depth=32768
#pragma HLS interface s_axilite port=buf.offset

    for (int b = 0; b < M; b++) {
        int *A = buf.read_offset();

#ifndef __SYNTHESIS__
        extern long ms();
        printf("[%5ld ms][kernel ] block %d: read_offset() -> offset=%-6ld region=%ld\n",
               ms(), b, (long)(A - buf.base), (long)(A[0] / TAG));
        fflush(stdout);
#endif

        for (int i = 0; i < N; i++) {
#ifndef __SYNTHESIS__
            extern void kernel_sleep_chunk(int i);
            kernel_sleep_chunk(i);
#endif
            out.write(A[i]);
        }
    }
}
