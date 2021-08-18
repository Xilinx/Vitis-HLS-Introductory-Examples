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

//                    +--proc-->[ II=4 ]--+
//                   /                     \               
// in -->[demux II=1]                       [mux II=1]---> out
//                   \                     /
//                    +--proc-->[ II=4 ]--+

#include "example.h"

//--------------------------------------------
template <int ID>
void proc_0(stream<int,16> &in, stream<int,16> &out)
{
    for (int i = 0; i < 25; i++)
    {
#pragma HLS PIPELINE II=4
#pragma HLS LATENCY min=2 max=2
        int var;
        in.read(var);
        out.write(var);
    }
}
void proc_1(stream<int,16> &in, stream<int> &out)
{
    for (int i = 0; i < 25; i++)
    {
#pragma HLS PIPELINE II=4
#pragma HLS LATENCY min=2 max=2
        int var;
        in.read(var);
        out.write(var);
    }
}

void proc_add(stream<int,16> &in, stream<int> &out)
{
    for (int i = 0; i < 25; i++)
    {
#pragma HLS PIPELINE II=4
#pragma HLS LATENCY min=2 max=2
        int var;
        in.read(var);
        out.write(var);
    }
}
//--------------------------------------------
void demux(stream<int> &in, stream<int,16> (&inter)[2])
{
    int demux_sel = 0;
    for (int i = 0; i < 50; i++)
    {
#pragma HLS PIPELINE II=1

        int var;
        in.read(var);
        inter[demux_sel].write(var);
        demux_sel = (demux_sel == 0) ? 1 : 0;
    }
}



void example(stream<int> &in, stream<int> &out, stream<int> &mux_in)
{
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS DATAFLOW

    stream<int,16> inter[2];
    stream<int,16> mux_in_lat;
    demux(in, inter);
    proc_0(inter[0], mux_in_lat);
    proc_1(inter[1], mux_in);
    proc_add(mux_in_lat, out);
}
