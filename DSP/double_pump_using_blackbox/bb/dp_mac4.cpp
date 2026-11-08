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
// C model of dp_mac4 -- the double-pumped 4-lane int8 MAC box.  csim links
// against this; bb/dp_mac4.v is the RTL that csynth/cosim use.  They MUST agree.
//
// PURE FUNCTION (no static state): one call per
// base cycle, lane l returns   w[2l]*x[2l] + w[2l+1]*x[2l+1]   as an int32.
//
// A TAG rides with the data, like a control byte: the byte
// that goes in at In[135:128] comes back at ap_return[135:128], delayed inside
// the RTL by exactly as long as the result, so it identifies WHICH call a result
// belongs to.  (In this C model there is no delay, so it just passes through.)
//
// PORT LAYOUT -- keep in step with dp_mac4.v and dp_top.cpp:
//     In     = {tag[7:0], Wpack[63:0], Xpack[63:0]}      136 bits, ONE port
//     Xpack  bit i*8 +: 8  = activation i  (int8), i = 0..7
//     Wpack  bit i*8 +: 8  = weight i      (int8), i = 0..7
//     return = {tag[7:0], lanes[127:0]}
//     lanes  bit l*32 +: 32 = lane l result (int32), l = 0..3
//     lane l, phase 0 uses element 2l, phase 1 uses element 2l+1.
#include <ap_int.h>

ap_uint<136> dp_mac4(ap_uint<136> In) {
    ap_uint<136> r = 0;
    for (int l = 0; l < 4; l++) {
        int s = 0;
        for (int ph = 0; ph < 2; ph++) {
            const int i = 2 * l + ph;
            const int x = (ap_int<8>)In.range(i * 8 + 7, i * 8);
            const int w = (ap_int<8>)In.range(64 + i * 8 + 7, 64 + i * 8);
            s += w * x;
        }
        r.range(l * 32 + 31, l * 32) = (ap_uint<32>)(ap_int<32>)s;
    }
    r.range(135, 128) = In.range(135, 128);
    return r;
}
