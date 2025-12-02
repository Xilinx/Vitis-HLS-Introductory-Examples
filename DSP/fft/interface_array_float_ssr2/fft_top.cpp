#include "fft_top.h"

void fft_top(bool direction, std::complex<data_in_t> in[FFT_LENGTH],
             std::complex<data_out_t> out[FFT_LENGTH], bool* ovflo) {
#pragma HLS interface ap_hs port = direction
#pragma HLS interface ap_fifo depth = 1 port = ovflo
#pragma HLS dataflow

    // no direction and no overflow for SSR>1
    hls::fft<config1>(in, out);

}

