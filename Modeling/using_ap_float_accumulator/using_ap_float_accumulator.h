#include <hls_stream.h>
#include <ap_float.h>

using apf_t = ap_float<18, 5>;
using acc_datatype_t = ap_fixed<32, 16>;
using stream_t = hls::stream<float>;
 
void using_ap_float_accumulator( stream_t &instream, stream_t &outstream, int n);

