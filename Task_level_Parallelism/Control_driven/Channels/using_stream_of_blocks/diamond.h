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
#pragma once
#include "hls_stream.h"
#include "hls_streamofblocks.h"

#define N 100
#define NUM_BLOCKS 10
typedef unsigned char data_t;
typedef data_t block_data_t[NUM_BLOCKS];

// Top function
void diamond(hls::stream<data_t>& vecIn, hls::stream<data_t>& vecOut);

// Sub functions
void funcA(hls::stream<data_t>& in, hls::stream_of_blocks<block_data_t>& out1,
           hls::stream_of_blocks<block_data_t>& out2);
void funcB(hls::stream_of_blocks<block_data_t>& in,
           hls::stream_of_blocks<block_data_t>& out);
void funcC(hls::stream_of_blocks<block_data_t>& in,
           hls::stream_of_blocks<block_data_t>& out);
void funcD(hls::stream_of_blocks<block_data_t>& in1,
           hls::stream_of_blocks<block_data_t>& in2, hls::stream<data_t>& out);
