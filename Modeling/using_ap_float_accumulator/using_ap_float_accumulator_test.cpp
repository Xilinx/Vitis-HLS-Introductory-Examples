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
#include "using_ap_float_accumulator.h"
#include <iostream>

void accumulator_float( stream_t &instream, stream_t &outstream, int n) {
    float acc=0;
accloop:
    for (unsigned int i = 0; i < n; i++) {
        acc += instream.read();
    }
    outstream.write(acc);
}

bool close_enough(float a, float b) {
    const float epsilon=(1+0.5/100); // 0.5% absolute difference
    if ( a>-0.0 ) {
        // positive
        if ( b < a*epsilon && a < b*epsilon ) return true; else return false;
    } else {
        // negative, changing order
        if ( b > a*epsilon && a > b*epsilon ) return true; else return false;
    }
}


int main(void) {
    const int num=1000,runs=4;
    int errors=0;
    stream_t tb_instream, tb_outstream;
    float results[runs];

    std::cout << "(i) init" << std::endl;
    for (int run = 1; run <= runs; run++) {
        for (int i = 1; i <= num; i++) {
            tb_instream.write(i/1000.f);
        }
        std::cout << "(i) run ("<<run<<")";
        if(run==1) {
            accumulator_float(tb_instream, tb_outstream,num);
            std::cout << " result golden: ";
        } else {
            using_ap_float_accumulator(tb_instream, tb_outstream,num);
            std::cout << " result DUT: ";
        }
        results[run]=tb_outstream.read();
        if(run!=1) {
            if ( !close_enough(results[1],results[run]) ) {
                std::cout << "!! too far !! ";
                errors++;
            }
        }
        std::cout << results[run] << std::endl;
    }
   
    if ( errors ) {
        std::cout << "(!) EXIT_FAILURE with " << errors << (errors==1?" error":" errors") << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "(i) EXIT_SUCCESS" << std::endl;
        return EXIT_SUCCESS;
    }
}
