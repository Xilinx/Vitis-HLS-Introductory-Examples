/*
 * Copyright 2021 Xilinx, Inc.
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

#include "diamond.h"
#define NUM_WORDS 16
extern "C" {

void diamond(vecOf16Words* vecIn, vecOf16Words* vecOut, int size)
{
  hls::stream<vecOf16Words> c0, c1, c2, c3, c4, c5;
  assert(size % 16 == 0);

  #pragma HLS dataflow
  load(vecIn, c0, size);
  compute_A(c0, c1, c2, size);
  compute_B(c1, c3, size);
  compute_C(c2, c4, size);
  compute_D(c3, c4,c5, size);
  store(c5, vecOut, size);
}
}

void load(vecOf16Words *in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    out << in[i];
  }
}

void compute_A(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out1, hls::stream<vecOf16Words >& out2, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    vecOf16Words t = in.read();
    out1 << t * 3;
    out2 << t * 3;
  }
}

void compute_B(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    out << in.read() + 25;
  }
}


void compute_C(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (data_t i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    out << in.read() * 2;
  }
}
void compute_D(hls::stream<vecOf16Words >& in1, hls::stream<vecOf16Words >& in2, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (data_t i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    out<< in1.read() + in2.read();
  }
}

void store(hls::stream<vecOf16Words >& in, vecOf16Words *out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
#pragma HLS pipeline II=1 rewind
//#pragma HLS pipeline rewind
//#pragma HLS unroll factor = 2
    out[i] = in.read();
  }
}
