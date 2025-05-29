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
#include "systolic_fir.h"

template <int size>
class FIR{
     private:
         const long* coeff_;
         long bias_;
         cascade<REG_A1|REG_B1|       REG_M|REG_P > dsp0;
         cascade<REG_A1|REG_B1|REG_B2|REG_M|REG_P > dsp_full[size - 1];
     public:
        FIR(const long* coeff, long bias)
            : coeff_(coeff), bias_(bias) {
#pragma HLS ARRAY_PARTITION variable=dsp_full complete dim=1
            };

        long fir(B_t input){

            auto out = dsp0.mul_add(coeff_[0], input, bias_);
            for(int j=1; j < size ; j++){
#pragma HLS unroll
                out = dsp_full[j - 1].mul_add(coeff_[j], out.bcout, out.pcout);
            }
            return out.pcout;
        };

};

void systolic_fir(

        long a[TAPS],
        long b[N], long c, long hw[N])
{
    #pragma HLS ARRAY_PARTITION variable=a complete dim=1
    FIR<TAPS> my_fir(a, c);

    LOOP_FIR:
    for(int i = 0 ; i< N ; i++){
        hw[i] = my_fir.fir(b[i]);
    }
}

