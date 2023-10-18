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

#include "hls_burst_maxi.h"

#ifndef _LOOP_SEQUENTIAL_H_
#define _LOOP_SEQUENTIAL_H_

#include "ap_int.h"
#define N 800

typedef ap_int<30> din_t;
typedef ap_int<20> dout_t;

typedef struct {
    din_t A;
    dout_t B;
} DUO;

void example(hls::burst_maxi<din_t> A, hls::burst_maxi<din_t> B,
             hls::burst_maxi<dout_t> RES, din_t factor);

#endif
