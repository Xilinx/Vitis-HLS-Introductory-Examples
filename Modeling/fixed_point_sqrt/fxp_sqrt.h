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

#ifndef __FXP_SQRT_H__
#define __FXP_SQRT_H__

#include <ap_fixed.h>
#include <cassert>
using namespace std;

// Fixed point square-root template
//
// Basic usage: fxp_sqrt(root_var, radicand_var);
//          or: fxp_sqrt<W2,IW2,W1,IW1>(root_var, radicand_var);
// where root_var and radicand_var are ap_ufixed<> variables and W2 & IW2
// are the total and iteger widths for root_var and W1/IW1 those for
// radicand_var.
//
// Despcription:
// This template function implements a non-restoring algorithm to calculate
// the square-root of a fixed point variable (ap_ufixed<>) of arbitrary
// format and assign it to another fixed point vaiable of arbitrary format.
//
// The template parameters specify the ap_ufixed<> width parameters to be used
// for input and output formats (see documentation for ap_fixed<> for details).
// These parameters do not need to be explicitly specified, as they will be
// inferred by the types passed to the function call.
//
// As this function is only intended for use with real numbers, the input
// and output variables are expected to be unsigned, i.e. ap_ufixed<>.  If
// signed types are passed, it is the user's responsibility to ensure the
// input variable does not go negative, as no checks are made for such;
// also note that passing a signed variable may result in slighty sub-optimal
// HLS due to a wider format than necessary.  Explicit casting is preferred.
//
// This function returns a precise (rounded to one ulp) result
// as long as the output format has at least half (rounded up) as many
// fractional bits as the input format.  The output format must have enough
// whole (integer) bit to hold thd full range of the result (i.e. half
// rounded up), otherwise an assertion failure will result at runtime (of SW
// model and/or RTL co-simulation).
//
// Note that integer square-roots may be accomplished with this function by
// use of ap_ufixed<> intermediate variables with no fractional portion, e.g.
//    uint16_t a;
//    uint8_t a_sqrt;
//    ...
//    ap_ufixed<16,16> a_fxp = ap_ufixed<16,16>(a);
//    ap_ufixed<8,8> a_sqrt_fxp;
//    fxp_sqrt(a_sqrt_fxp, a_fxp);
//    a_sqrt = a_sqrt_fxp.toint();
//
// Which should result in no more hardware area than having an integer-
// specific version of this function.

template <int W2, int IW2, int W1, int IW1>
void fxp_sqrt(ap_ufixed<W2, IW2>& result, ap_ufixed<W1, IW1>& in_val) {
    enum { QW = (IW1 + 1) / 2 + (W2 - IW2) + 1 }; // derive max root width
    enum {
        SCALE = (W2 - W1) - (IW2 - (IW1 + 1) / 2)
    }; // scale (shift) to adj initial remainer value
    enum { ROOT_PREC = QW - (IW1 % 2) };
    assert((IW1 + 1) / 2 <=
           IW2); // Check that output format can accommodate full result

    ap_uint<QW> q = 0;      // partial sqrt
    ap_uint<QW> q_star = 0; // diminished partial sqrt
    ap_int<QW + 2> s; // scaled remainder initialized to extracted input bits
    if (SCALE >= 0)
        s = in_val.range(W1 - 1, 0) << (SCALE);
    else
        s = ((in_val.range(W1 - 1, 0) >> (0 - (SCALE + 1))) + 1) >> 1;

    // Non-restoring square-root algorithm
    for (int i = 0; i <= ROOT_PREC; i++) {
        if (s >= 0) {
            s = 2 * s - (((ap_int<QW + 2>(q) << 2) | 1) << (ROOT_PREC - i));
            q_star = q << 1;
            q = (q << 1) | 1;
        } else {
            s = 2 * s +
                (((ap_int<QW + 2>(q_star) << 2) | 3) << (ROOT_PREC - i));
            q = (q_star << 1) | 1;
            q_star <<= 1;
        }
    }
    // Round result by "extra iteration" method
    if (s > 0)
        q = q + 1;
    // Truncate excess bit and assign to output format
    result.range(W2 - 1, 0) = ap_uint<W2>(q >> 1);
}

#endif //__FXP_SQRT_H__ not defined
