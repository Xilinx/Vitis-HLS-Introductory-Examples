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
#include "dp_top.h"

// One call of the box per iteration, II=1: 8 MAC/cycle on 4 multipliers that
// run at 2x.  x_mem / w_mem are ap_memory with a 64-bit word, so one read per
// array per cycle supplies all 8 operands -- no partitioning needed.
//
// Each call carries a TAG = the iteration index.  The box returns it beside the
// result, delayed like the result, so `tag != i` means this iteration read a
// result that belongs to a different call.  Mismatches are counted into
// out[4] / out[5]; the sums are accumulated either way.
void dp_top(const ap_uint<64> x_mem[NW], const ap_uint<64> w_mem[NW], ap_int<32> out[6]) {
#pragma HLS interface mode=ap_memory port=x_mem
#pragma HLS interface mode=ap_memory port=w_mem
#pragma HLS interface mode=ap_memory port=out

    ap_int<32> acc[4] = {0, 0, 0, 0};
#pragma HLS array_partition variable=acc complete dim=1
    ap_int<32> n_bad = 0;
    ap_int<32> first_bad = -1;

    for (int i = 0; i < NW; i++) {
#pragma HLS pipeline II=1
        // {tag[135:128], W[127:64], X[63:0]} -- same layout as bb/dp_mac4.{cpp,v}.
        // Input take_effect_stage is 1 (bb/dp_mac4.json), so HLS samples the memory data.
        const ap_uint<8> tag_in = (ap_uint<8>)i;
        const ap_uint<136> in = (tag_in, w_mem[i], x_mem[i]);
        const ap_uint<136> r = dp_mac4(in);
        for (int l = 0; l < 4; l++)
            acc[l] += (ap_int<32>)r.range(l * 32 + 31, l * 32);
        if (r.range(135, 128) != tag_in) {
            n_bad += 1;
            if (first_bad < 0) first_bad = i;
        }
    }
    for (int l = 0; l < 4; l++) out[l] = acc[l];
    out[4] = n_bad;
    out[5] = first_bad;
}
