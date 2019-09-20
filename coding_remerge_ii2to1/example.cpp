//                    +--proc-->[ II=2 ]--+
//                   /                     \               
// in -->[demux II=1]                       [mux II=1]---> out
//                   \                     /
//                    +--proc-->[ II=2 ]--+

#include "example.h"

//--------------------------------------------
template <int ID>
void proc(stream<int> &in, stream<int> &out)
{
    for (int i = 0; i < 25; i++)
    {
#pragma HLS PIPELINE II=2
#pragma HLS LATENCY min=2 max=2
        int var;
        in.read(var);
        out.write(var);
    }
}

//--------------------------------------------
void mux(stream<int> (&inter)[2], stream<int> &mux_output)
{
    int mux_sel = 0;
    for (int i = 0; i < 50; i++)
    {
#pragma HLS PIPELINE II=1
        int var;
        inter[mux_sel].read(var);
        mux_output.write(var);
        mux_sel = (mux_sel == 0) ? (1) : (0);
    }
}

//--------------------------------------------
void demux(stream<int> &in, stream<int> (&inter)[2])
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

void example(stream<int> &in, stream<int> &out)
{
#pragma HLS DATAFLOW

    stream<int> inter[2];
    stream<int> mux_in[2];

#pragma HLS STREAM variable = inter depth = 16
#pragma HLS STREAM variable = mux_in depth = 16

    demux(in, inter);
    proc<0>(inter[0], mux_in[0]);
    proc<1>(inter[1], mux_in[1]);
    mux(mux_in, out);
}