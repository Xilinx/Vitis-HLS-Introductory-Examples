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
#include <iostream>
#include <fstream>
#include <cmath>
#include "fft_top.h"

using namespace std;

// Raw value of the 13-bit scaling factor (UFIX_13_13): the scaling factor is
// SCALING_RAW / 8192. With SCALING_RAW = 256 the factor is 1/32 = 1/FFT_LENGTH.
// The forward FFT is then scaled by 1/N and the unscaled inverse FFT restores
// the amplitude, so the output of the design is equal to its input.
const int SCALING_RAW = 256;

int main()
{
    const double TOLERANCE = 1e-3;

    // Read the stimulus: one complex sample per line (real and imaginary parts)
    double input_re[FFT_LENGTH], input_im[FFT_LENGTH];
    ifstream stimfile("data/input_n32.txt");
    if (!stimfile.is_open()) {
        cerr << "ERROR: Can't open data/input_n32.txt" << endl;
        return 1;
    }
    for (int i = 0; i < FFT_LENGTH; i++) {
        stimfile >> input_re[i] >> input_im[i];
    }
    stimfile.close();

    hls::stream<input_t>  xn;
    hls::stream<output_t> xk;

    // Write one frame to the input stream. The golden data are the input samples
    // after conversion to the input data type (16 bits).
    fwd_in_t::data_in_t golden_re[FFT_LENGTH], golden_im[FFT_LENGTH];
    for (int i = 0; i < FFT_LENGTH / config_fwd::SAMPLE_RATE; i++) {
        input_t in;
        for (int j = 0; j < config_fwd::SAMPLE_RATE; j++) {
            const int n = i * config_fwd::SAMPLE_RATE + j;
            in.tdata[j].real(input_re[n]);
            in.tdata[j].imag(input_im[n]);
            golden_re[n] = in.tdata[j].real();
            golden_im[n] = in.tdata[j].imag();
        }
        // Frame control values, set on every sample of the frame
        in.setPointSize(FFT_NFFT);  // log2 of the number of points (5 = 32 points)
        in.setDirection(true);      // true: forward FFT
        in.setScalingFactor(SCALING_RAW);
        // TLAST marks the last word of the frame
        in.tlast = (i == (FFT_LENGTH / config_fwd::SAMPLE_RATE) - 1);
        xn.write(in);
    }

    // Simulate the forward and inverse FFT: one frame for each call
    fft_top(xn, xk);

    // Read the output frame and put the samples in natural order using the index
    output_t::cplx_data_out_t xk_natural[FFT_LENGTH];
    bool bin_seen[FFT_LENGTH] = {false};
    int error_num = 0;
    for (int i = 0; i < FFT_LENGTH / config_inv::SAMPLE_RATE; i++) {
        output_t out = xk.read();
        for (int j = 0; j < config_inv::SAMPLE_RATE; j++) {
            unsigned index = out.getIndex(j);
            if (index >= FFT_LENGTH || bin_seen[index]) {
                error_num++;
                cout << "Invalid or repeated sample index: " << index << endl;
                continue;
            }
            bin_seen[index] = true;
            xk_natural[index] = out.tdata[j];
        }
    }

    // Compare with the golden data
    for (int i = 0; i < FFT_LENGTH; i++) {
        double out_re = xk_natural[i].real().to_double();
        double out_im = xk_natural[i].imag().to_double();
        if (fabs(out_re - golden_re[i].to_double()) > TOLERANCE ||
            fabs(out_im - golden_im[i].to_double()) > TOLERANCE) {
            error_num++;
            cout << "Index: " << i
                 << "  Golden: (" << golden_re[i].to_double() << ", " << golden_im[i].to_double() << ")"
                 << " vs. Output: (" << out_re << ", " << out_im << ")" << endl;
        }
    }

    cout << " ERRORS: " << error_num << endl;
    if (error_num > 0)
        cout << " (FAILED!!!)" << endl;
    else
        cout << " (PASSED!!!)" << endl;

    return error_num > 0;
}
