/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "adder_top.h"

#define N 25
int main() {

    mystream_in in1;
    mystream_in in2;
    mystream_out out;
    stream_vld_in   vIn;
    stream_vld_out vOut;
    int scalarInput = 1;
    int golden[N];
    for (int i = 0; i < N; i++)
    {
        in1.write(i);
        in2.write(i);
        vIn.write(1);
        golden[i] = 2*i;
    }
     vIn.write(0);
     vIn.write(0);
     vIn.write(0);
    adder_top(in1, in2, out,vIn,vOut);

    for(int i=0;i<N;i++)
    {
        int var = vOut.read();
        if(var == 1)
        {   
            if(out.read() != golden[i])
                return 1;
        }
    }

    return 0;
}