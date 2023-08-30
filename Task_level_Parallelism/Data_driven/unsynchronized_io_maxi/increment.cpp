/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define HLS_TASK_ALLOW_NON_STREAM_ARGS
#include "hls_task.h"
#define DWIDTH 32
typedef ap_axiu<DWIDTH, 0, 0, 0> trans_pkt;
#include <iostream>
void process_11(hls::stream<trans_pkt >& input,
int *out, hls::stream<int>& int_fifo)
{

bool eos = false;
int counter =0;     
  do{
 
 #pragma HLS PIPELINE II=1 style=flp     
            trans_pkt t2 = input.read();
            ap_uint<DWIDTH> in2 = t2.data;
            out[counter++] = in2;
            int_fifo.write(in2);
            if (t2.last) {
               eos = true;
            }
        } while (eos == false);
    
}

void process_23(hls::stream<int>& int_fifo,
int *in,
hls::stream<trans_pkt>& output)
{
	static int val;

for(int i=0;i<1024;i++)
{
  #pragma HLS PIPELINE II=1 style=flp     

  trans_pkt t_out;
    if(i == 1023)
    {
        t_out.last = 1;
    }
    int_fifo.read(val);
  ap_uint<DWIDTH> in2 = in[i]+val;
  std::cout << "value of in1 "<< in[i] << " value of val is " << val << std::endl;
  t_out.data = in2; 
  output.write(t_out);   
}

}
extern "C" {
void increment(hls::stream<trans_pkt >& input, hls::stream<trans_pkt >& output, int *out, int *in)
{
#pragma HLS DATAFLOW
#pragma HLS stable variable=in
#pragma HLS stable variable=out
#pragma HLS INTERFACE m_axi port=in bundle=gmem depth=1024
#pragma HLS INTERFACE m_axi port=out bundle=gmem depth=1024
#pragma HLS INTERFACE mode=s_axilite port=in
#pragma HLS INTERFACE mode=s_axilite port=out


hls_thread_local hls::stream<int> int_fifo("int_fifo");
#pragma HLS STREAM depth=64 type=fifo variable=int_fifo

hls_thread_local hls::task t1(process_11,input,out, int_fifo);
hls_thread_local hls::task t2(process_23,int_fifo,out, output );

}
}
