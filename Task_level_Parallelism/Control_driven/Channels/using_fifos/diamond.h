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

typedef unsigned int data_t;
#include <hls_stream.h>
#include <hls_vector.h>
#include <iostream>
#include <vector>

#define NUM_WORDS 16

// Each vector will be 64 bytes (16 x 4 bytes)
typedef hls::vector<uint32_t, NUM_WORDS> vecOf16Words;

extern "C" {
// Top function
void diamond(vecOf16Words* vecIn, vecOf16Words* vecOut, int size);
}

// Sub functions

void load(vecOf16Words* in, hls::stream<vecOf16Words>& out, int vSize);
void compute_A(hls::stream<vecOf16Words>& in, hls::stream<vecOf16Words>& out1,
               hls::stream<vecOf16Words>& out2, int vSize);
void compute_B(hls::stream<vecOf16Words>& in, hls::stream<vecOf16Words>& out,
               int vSize);
void compute_C(hls::stream<vecOf16Words>& in, hls::stream<vecOf16Words>& out,
               int vSize);
void compute_D(hls::stream<vecOf16Words>& in1, hls::stream<vecOf16Words>& in2,
               hls::stream<vecOf16Words>& out, int vSize);
void store(hls::stream<vecOf16Words>& in, vecOf16Words* out, int vSize);
