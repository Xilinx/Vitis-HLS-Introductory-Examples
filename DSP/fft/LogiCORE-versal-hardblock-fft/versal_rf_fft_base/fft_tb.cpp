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
#include <cmath>
#include "fft_top.h"

using namespace std;

// Raw value of the 13-bit scaling factor (UFIX_13_13): the scaling factor is
// SCALING_RAW / 8192. With SCALING_RAW = 4 the factor is 1/2048 = 1/FFT_LENGTH,
// so a tone of amplitude 0.5 gives a peak of amplitude 0.5 (no overflow).
const int SCALING_RAW = 4;

int main()
{
    const double PI = 3.14159265358979323846;
    const double TOLERANCE = 1e-3;

    // Each frame is a complex tone x[n] = 0.5 * exp(+j*2*pi*k*n/N).
    // The forward FFT of this tone has one non-zero bin, X[k] = 0.5 * N * SCALING_RAW / 8192.
    // The bin numbers differ for each frame: the output is in bit-reversed order,
    // so a bin different from 0 checks that the index of the samples is used correctly.
    const int NUM_FRAMES = 3;
    const int tone_bin[NUM_FRAMES] = {100, 7, 1500};

    int error_num = 0;
    hls::stream<input_t>  xn;
    hls::stream<output_t> xk;

    for (int frame = 0; frame < NUM_FRAMES; ++frame) {
        const int k = tone_bin[frame];

        // Write one frame to the input stream
        for (int i = 0; i < FFT_LENGTH / config::SAMPLE_RATE; ++i) {
            input_t in;
            for (int j = 0; j < config::SAMPLE_RATE; ++j) {
                const int n = i * config::SAMPLE_RATE + j;
                in.tdata[j].real(0.5 * cos(2.0 * PI * k * n / FFT_LENGTH));
                in.tdata[j].imag(0.5 * sin(2.0 * PI * k * n / FFT_LENGTH));
            }
            // Frame control values, set on every sample of the frame
            in.setPointSize(FFT_NFFT); // log2 of the number of points (11 = 2048 points)
            in.setDirection(true);     // true: forward FFT, false: inverse FFT
            in.setScalingFactor(SCALING_RAW);
            // TLAST marks the last word of the frame
            in.tlast = (i == (FFT_LENGTH / config::SAMPLE_RATE) - 1);
            xn.write(in);
        }

        // Simulate the FFT: one frame for each call
        fft_top(xn, xk);

        // Read the output frame and put the bins in natural order using the index
        output_t::cplx_data_out_t xk_natural[FFT_LENGTH];
        bool bin_seen[FFT_LENGTH] = {false};
        for (int i = 0; i < FFT_LENGTH / config::SAMPLE_RATE; ++i) {
            output_t out = xk.read();
            for (int j = 0; j < config::SAMPLE_RATE; ++j) {
                unsigned index = out.getIndex(j); // frequency bin of this sample
                if (index >= FFT_LENGTH || bin_seen[index]) {
                    error_num++;
                    cout << "Frame:" << frame << " invalid or repeated bin index: " << index << endl;
                    continue;
                }
                bin_seen[index] = true;
                xk_natural[index] = out.tdata[j];
            }
        }

        // Compare with the expected result
        for (int i = 0; i < FFT_LENGTH; ++i) {
            double golden_re = (i == k) ? 0.5 * FFT_LENGTH * SCALING_RAW / 8192.0 : 0.0;
            double golden_im = 0.0;
            double out_re = xk_natural[i].real().to_double();
            double out_im = xk_natural[i].imag().to_double();
            if (fabs(out_re - golden_re) > TOLERANCE || fabs(out_im - golden_im) > TOLERANCE) {
                error_num++;
                cout << "Frame:" << frame << " bin: " << i
                     << "  Golden: (" << golden_re << ", " << golden_im << ")"
                     << " vs. Output: (" << out_re << ", " << out_im << ")" << endl;
            }
        }
    }

    cout << " ERRORS: " << error_num << endl;
    if (error_num > 0)
        cout << " (FAILED!!!)" << endl;
    else
        cout << " (PASSED!!!)" << endl;

    return error_num > 0;
}
