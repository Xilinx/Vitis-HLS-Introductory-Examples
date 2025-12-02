#include "fft_top.h"

void fft_top(ap_uint<1> dir,
             ap_uint<15> scaling,
             hls::stream<cmpxDataIn> &xn,
             hls::stream<cmpxDataOut> &xk,
             bool* status)
{
#pragma HLS interface ap_fifo depth=1 port=status
#pragma HLS interface ap_fifo depth=1024 port=xn,xk
#pragma HLS stream variable=xn
#pragma HLS stream variable=xk
#pragma HLS dataflow

    hls::fft<config1>(xn, xk, dir, scaling, -1, status);

}

