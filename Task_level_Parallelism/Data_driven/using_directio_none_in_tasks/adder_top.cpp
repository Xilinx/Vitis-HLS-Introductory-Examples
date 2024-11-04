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
void addInputs(mystream_in &in, mystream_in &dinB,stream_vld_in&vldIn,mystream_out &dout_Y,stream_vld_out & vldOut)
{
            #pragma HLS pipeline II=1
            int var = vldIn.read();
            if(var ==0)
            {
              vldOut.write(var);
              return;
            }
            int tmp1 =  in.read(); // Blocking_read
            int tmp2 =  dinB.read(); // Blocking read
            int out = tmp1+ tmp2;
            dout_Y.write(out);
            vldOut.write(1);
    }

void adder_top(mystream_in &din_A, mystream_in &din_B, mystream_out &dout_Y,stream_vld_in &vldIn, stream_vld_out & vldOut) {

    hls_thread_local hls::task t1(addInputs,din_A,din_B,vldIn,dout_Y,vldOut);

}

