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
#include "unaligned_burst_rgb.h"

void unaligned_burst_rgb(hls::burst_maxi<ap_uint<512>> in,
                         hls::burst_maxi<ap_uint<512>> out,
                         unsigned n) {
    // MAX_ELEMENTS rgb_t pixels go through a 512-bit port, so compute the real depth value
    #pragma HLS interface m_axi port=in  bundle=gmem0 depth=MAX_ELEMENTS*sizeof(rgb_t)/(512/8)
    #pragma HLS interface m_axi port=out bundle=gmem1 depth=MAX_ELEMENTS*sizeof(rgb_t)/(512/8)

    // Manual burst request for the unaligned, wide-batch path:
    // groups of 16 rgb_t (24-bit, non-power-of-2) packed into a 384-bit access
    in.read_request_unaligned<hls::vector<rgb_t, 16>>(0, n / 16);
    out.write_request_unaligned<hls::vector<rgb_t, 16>>(0, n / 16);

    // Manual burst request for the unaligned tail path: remaining elements one at a time
    int remaining = n % 16;
    in.read_request_unaligned<rgb_t>(n / 16 * sizeof(hls::vector<rgb_t, 16>), remaining);
    out.write_request_unaligned<rgb_t>(n / 16 * sizeof(hls::vector<rgb_t, 16>), remaining);

mainwideburstloop:
    for (int i = 0; i < n / 16; i++) {
        #pragma HLS pipeline II = 1
        hls::vector<rgb_t, 16> v = in.read<hls::vector<rgb_t, 16>>();
        out.write<hls::vector<rgb_t, 16>>(v);
    }
    out.write_response();
    // Note: n/16 does not translate 1:1 into n/16 M_AXI bursts, because
    // vector<rgb_t,16> is 24*16=384 bits -- narrower than the 512-bit port.
    // K vector<rgb_t,16> accesses might take less than K 512-bits accesses.
    // e.g. with n=100: 100/16 = 6 vectors<16> = 6*384 = 2304 bits
    //      2304/512 = 4.5 -> needs 5 M_AXI accesses of 512 bits so burst ARLEN=4
    //      100%16 = 4 remaining elements handled by the tail loop below with ARLEN=0

lastnarrowburstloop:
    for (int i = 0; i < remaining; i++) {
        #pragma HLS pipeline II = 1
        out.write<rgb_t>(in.read<rgb_t>());
    }
    out.write_response();
}
