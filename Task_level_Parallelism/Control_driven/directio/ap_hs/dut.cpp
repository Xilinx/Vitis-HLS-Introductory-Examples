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
#include "dut.h"

void krnl_stream_vdatamover(hls::stream<pkt> &in,
                      hls::stream<pkt> &out,
					            int mem[DATA],
                      hls::ap_hs<int> &reset_value, // output
                      hls::ap_hs<int> &reset_myCounter // input 
                      ) {
#pragma HLS interface mode=axis port=in
#pragma HLS interface mode=axis port=out
#pragma HLS INTERFACE m_axi port = mem 

static int counter; 
bool eos = false;

pkt t_out;
for(int i=0;i<DATA;i++)
{
  ap_uint<DWIDTH> tmpOut;
      pkt t1 = in.read();
    if(reset_myCounter.valid())
    {
      int reset = reset_myCounter.read();
      if(reset == 1)
      {
        ap_uint<DWIDTH> i1 = t1.data;
        tmpOut = i1+mem[i];
        t_out.data = tmpOut;
        t_out.last = t1.last;
        out.write(t_out);
        counter =0;
        reset_value.write(counter);
        break;
      }
    }

    if(counter==threshold)
    {
      reset_value.write(counter);
      break;
    }
    counter++;
    // Packet for output
    
    reset_value.write(counter);
    // Reading data from input packet
    ap_uint<DWIDTH> in1 = t1.data;

    // Vadd operation
     tmpOut = in1+mem[i];

    // Setting data and configuration to output packet
    t_out.data = tmpOut;
    t_out.last = t1.last;
    t_out.keep = -1; // Enabling all bytes

    // Writing packet to output stream
    out.write(t_out);

    if (t1.last) {
      eos = true;
    }
}
 }
