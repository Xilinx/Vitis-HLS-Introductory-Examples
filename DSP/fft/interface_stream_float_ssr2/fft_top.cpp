#include "fft_top.h"

void fft_top(bool direction, hls::stream<hls::vector<cmpxDataIn, FFT_SSR>> &in,
             hls::stream<hls::vector<cmpxDataOut, FFT_SSR>> &out, bool* ovflo) {
#pragma HLS interface ap_hs port = direction
#pragma HLS interface ap_fifo depth = 1 port = ovflo
#pragma HLS dataflow

    // no direction and no overflow for SSR>1
    hls::fft<config1>(in, out);

}

