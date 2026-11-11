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
#ifndef DP_TOP_H
#define DP_TOP_H
#include <ap_int.h>

// 256 words x 8 int8 = 2048 elements per vector.  Fixed so csim/cosim are
// deterministic.  Each word packs 8 int8 (element i at bits i*8 +: 8).
static const int NW = 256;

// out[0..3] = lane l total over all words of  w[2l]*x[2l] + w[2l+1]*x[2l+1]  (int32)
// out[4]    = number of iterations whose returned tag != the iteration index
// out[5]    = first iteration with a tag mismatch, or -1
void dp_top(const ap_uint<64> x_mem[NW], const ap_uint<64> w_mem[NW], ap_int<32> out[6]);

// The double-pumped RTL black box (bb/dp_mac4.{v,cpp,json}).
// In = {tag, Wpack, Xpack}; return = {tag, 4 x int32 lane results}.
extern ap_uint<136> dp_mac4(ap_uint<136> In);

#endif
