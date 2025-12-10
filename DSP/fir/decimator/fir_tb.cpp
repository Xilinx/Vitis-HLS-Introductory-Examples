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
#include "fir_top.h"
#include <iostream>
#define N 3

int main()
{
    s_data_t in[INPUT_LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    m_data_t out[OUTPUT_LENGTH] = {0};
    int err=0; 
    m_data_t ref[INPUT_LENGTH * 3] = {
    0,6,12,14,13,17,27,31,35,39,43,47,51,55,59,63,67,71,75,79,
    0,2640,0,760,0,20,0,560,0,840,0,1120,0,1400,0,1680,0,1960,0,2240,
    0,57,0,25,0,113,0,1,0,9,0,17,0,25,0,33,0,41,0,49
    };
    for (int j = 0; j < N; ++j) {
        config_t config = j;
        fir_top(in, out, &config); 
        for(unsigned i = 0; i < OUTPUT_LENGTH; ++i)
        {
            std::cout << "out[" << i << "] in " << j << " is " << out[i] << " -- expected value is " <<  ref[i*2+1+INPUT_LENGTH*j] 
                << std::endl;
            if(out[i] != ref[i*2+1 + INPUT_LENGTH * j])
            {
                err++;
            }
        }
    }
    
    if (err > 0 )
        printf ("FAIL!!!!\n");
    else
        printf ("PASS!!!!\n");
    return err;
}
