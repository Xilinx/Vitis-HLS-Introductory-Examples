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
#ifndef __AUTORESTART_USING_MAXI_BUF_TEST_H__
#define __AUTORESTART_USING_MAXI_BUF_TEST_H__

#include "hls_top.h"
#include "hls_directio.h"
#include "hls_stream.h"

#define N        8192        // samples per block = one read_offset()
#define REGIONS  4           // regions living in the MAXI buffer
#define M        8           // read_offset() calls per invocation
#define TOTAL    (N * M)     // samples moved in ONE kernel run
#define TAG      1000000

// csim-only pacing: sleep CHUNK_MS every CHUNK samples.
#define CHUNK    1024
#define CHUNK_MS 20
#define PUSH_MS  120*2

HLS_TOP_AUTORESTART(my_dut,
                    hls::m_axi_buf<int, hls::ap_none<size_t>> buf,
                    hls::stream<int> &out);

#ifndef __SYNTHESIS__
long ms();
void kernel_sleep_chunk(int i);
#endif

#endif
