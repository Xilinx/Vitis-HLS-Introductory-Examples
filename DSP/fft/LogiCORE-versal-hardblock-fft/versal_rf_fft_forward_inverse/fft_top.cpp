/*
 * Copyright 2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "fft_top.h"

// Converts the words of the forward FFT output to the words of the inverse FFT input.
// The data types are identical (ap_fixed<18,1>). Only the frame control values
// (point size and direction) are added. The scaling factor does not exist for an
// unscaled FFT.
static void fwd_to_inv(hls::stream<fwd_out_t> &xmid,
                       hls::stream<inv_in_t> &xmid_inv)
{
    for (int i = 0; i < FFT_LENGTH / config_fwd::SAMPLE_RATE; i++) {
#pragma HLS pipeline II=1
        fwd_out_t a = xmid.read();
        inv_in_t b;
        for (int j = 0; j < config_inv::SAMPLE_RATE; j++) {
            b.tdata[j] = a.tdata[j];
        }
        b.setPointSize(FFT_NFFT);
        b.setDirection(false);   // inverse FFT
        b.tlast = a.tlast;
        xmid_inv.write(b);
    }
}

void fft_top(hls::stream<input_t> &xn,
             hls::stream<output_t> &xk)
{
#pragma HLS dataflow

    hls::stream<fwd_out_t> xmid;
    hls::stream<inv_in_t> xmid_inv;

    hls::vrf_fft::fft<config_fwd>(xn, xmid);
    fwd_to_inv(xmid, xmid_inv);
    hls::vrf_fft::fft<config_inv>(xmid_inv, xk);

}
