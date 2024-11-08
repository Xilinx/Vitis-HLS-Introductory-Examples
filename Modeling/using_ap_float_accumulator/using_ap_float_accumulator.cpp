#include "using_ap_float_accumulator.h"

void using_ap_float_accumulator( stream_t &instream, stream_t &outstream, int n) {
    apf_t res;
    ap_float_acc<apf_t, acc_datatype_t> acc;
accloop:
    for (unsigned int i = 0; i < n; i++) {
        apf_t value = instream.read();
        res = acc.accumulate(value, i == n-1);
    }
    outstream.write((float)res);
}

