#include "ap_int.h"
#include "hls_burst_maxi.h"
const int NT = 10;

void transfer_kernel(hls::burst_maxi<int> in,hls::burst_maxi<int> out, const int size );
