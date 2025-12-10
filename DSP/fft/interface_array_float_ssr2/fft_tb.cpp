/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define BUF_SIZE 64
int main() {
    const int SIM_FRAMES = 10;
    const int SAMPLES = (1 << FFT_NFFT_MAX);

    int error_num = 0;
    bool ovflo_all = false;
    char res_filename[BUF_SIZE] = {0};
    char dat_filename[BUF_SIZE] = {0};
    static cmpxDataIn xn_input[SAMPLES];
    static cmpxDataOut xk_output[SAMPLES];

    for(int i = 0; i < SAMPLES; i++){
        xn_input[i] = cmpxDataIn(0,0);
        xk_output[i] = cmpxDataIn(0,0);
    }
    

    for (int frame = 0; frame < SIM_FRAMES; ++frame) {
        int NFFT = 0;
        int CP_LEN = 0; // length of the cyclic prefix to be inserted for each frame
        int FWD_INV = 0;
        int sc_sch = 0;
        int line_no = 1;
        FILE* stimfile;

        // Open stimulus .dat file for reading
        if(FFT_SSR==2) 
            sprintf(dat_filename, "data-ssr2/stimulus_%02d", frame);
        else
            sprintf(dat_filename, "data-ssr4/stimulus_%02d", frame);            
        strcat(dat_filename, ".dat");
        stimfile = fopen(dat_filename, "r");

        int tmp_re, tmp_im;
        float dummy_re, dummy_im;
        const int max = 1 << FFT_INPUT_WIDTH; // might not work for > 32 bits!
        const int max_half_minus_one = (max / 2) - 1;
        // Scaling factor to get integer into -1 <= x < +1 range
        const double sc =
            ldexp(1.0, FFT_INPUT_WIDTH - 1); // might not work for > 32 bits!

        if (stimfile == NULL) {
            printf("ERROR: Can't open %s\n", dat_filename);
            exit(999);
        } else {
            printf("INFO: Reading %s ", dat_filename);
            while (fgetc(stimfile) != EOF && line_no < SAMPLES + 5) {
                switch (line_no) {
                case 1:
                    // Point size
                    fscanf(stimfile, "%X", &NFFT);
                    printf("NFFT %d ", NFFT);
                    break;
                case 2:
                    // CP length
                    fscanf(stimfile, "%X", &CP_LEN);
                    printf("CP_LEN %3d ", CP_LEN);
                    break;
                case 3:
                    // fwd-inv
                    fscanf(stimfile, "%X", &FWD_INV);
                    printf("FWD_INV %d ", FWD_INV);
                    break;
                case 4:
                    // Scaling schedule sc_sch
                    fscanf(stimfile, "%X", &sc_sch);
                    printf("sc_sch %X\n", sc_sch);
                    break;
                default:
                    // hex data (first 2 columns)
                    fscanf(stimfile, "%x %x %f %f", &tmp_re, &tmp_im, &dummy_re,
                           &dummy_im);
                    // printf("%x %x\n",tmp_re,tmp_im);

                    float input_data_re, input_data_im;
                    std::memcpy(&input_data_re, &tmp_re, sizeof(input_data_re));
                    std::memcpy(&input_data_im, &tmp_im, sizeof(input_data_im));

                    xn_input[line_no - 5] =
                        cmpxDataIn(input_data_re, input_data_im);
                }
                line_no++;
            }
        }
        fclose(stimfile);

        bool ovflo;

        fft_top(FWD_INV, xn_input, xk_output, &ovflo);

        ovflo_all |= ovflo;

        // Open golden results .res file for reading
        FILE* resfile;
        if(FFT_SSR==2) 
            sprintf(res_filename, "data-ssr2/stimulus_%02d", frame);
        else
            sprintf(res_filename, "data-ssr4/stimulus_%02d", frame);            
        strcat(res_filename, ".res");
        if ((resfile = fopen(res_filename, "r")) == 0) {
            printf("ERROR: Can't open %s\n", res_filename);
            exit(888);
        }

        int tmp;
        fscanf(resfile, "%X", &tmp);
        fscanf(resfile, "%X", &tmp);
        for (int i = 0; i < (1 << NFFT); i++) {
            fscanf(resfile, "%x %x %f %f", &tmp_re, &tmp_im, &dummy_re,
                   &dummy_im);

            float output_data_re = 0, output_data_im = 0;
            std::memcpy(&output_data_re, &tmp_re, sizeof(output_data_re));
            std::memcpy(&output_data_im, &tmp_im, sizeof(output_data_im));
            
            if (output_data_re != xk_output[i].real()) {
                error_num++;
                cout << "Frame:" << frame << " index: " << i
                     << "  Golden: " << output_data_re
                     << " vs. RE Output: " << setprecision(14)
                     << xk_output[i].real() << endl;
            }
           
            if (output_data_im != xk_output[i].imag()) {
                error_num++;
                cout << "Frame:" << frame << " index: " << i
                     << "  Golden: " << output_data_im
                     << " vs. IM Output: " << setprecision(14)
                     << xk_output[i].imag() << endl;
            }
            
        }
        fclose(resfile);
    }

    cout << " ERRORS: " << error_num << endl;
    if (error_num > 0)
        cout << " (FAILED!!!)" << endl;
    else if (ovflo_all)
        cout << " (OVERFLOW!!!)" << endl;
    else
        cout << " (PASSED!!!)" << endl;

    if (error_num > 0)
        return 1;
    else
        return 0;
}
