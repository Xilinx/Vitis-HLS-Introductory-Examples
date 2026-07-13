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
#include "hls_burst_maxi.h"
#include "hls_vector.h"
#include "ap_int.h"

#define MAX_ELEMENTS 1024

struct rgb_t {
    char R;
    char G;
    char B;
};

void unaligned_burst_rgb(hls::burst_maxi<ap_uint<512>> in,
                         hls::burst_maxi<ap_uint<512>> out,
                         unsigned n);
