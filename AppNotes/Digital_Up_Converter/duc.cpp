#include "duc.h"

#if 1
//______________________________________________________________________________
void duc(DATA_T din_i[L_INPUT], DATA_T din_q[L_INPUT], DATA_T dout[L_OUTPUT],
         incr_t incr) {

#pragma HLS interface axis depth = L_INPUT port = din_i
#pragma HLS interface axis depth = L_INPUT port = din_q
#pragma HLS interface axis depth = L_OUTPUT port = dout

#pragma HLS STABLE variable = incr

#pragma HLS dataflow

    static filterStageClassTwoChan<L_INPUT> f0;
    static dds_class<L_OUTPUT, dds_t, DATA_T> dds_0;

    DATA_T dout_i[L_OUTPUT];
    DATA_T dout_q[L_OUTPUT];
    dds_t dds_cos[L_OUTPUT];
    dds_t dds_sin[L_OUTPUT];

    f0.process(din_i, dout_i, din_q, dout_q);
    dds_0.process_frame(incr, dds_cos, dds_sin);
    dds_0.mix(dds_cos, dds_sin, dout_i, dout_q, dout);
}

#endif
