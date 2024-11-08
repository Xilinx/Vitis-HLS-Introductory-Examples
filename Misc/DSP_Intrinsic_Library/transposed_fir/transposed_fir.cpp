#include "transposed_fir.h"

// using actual bitwidth of DSP slice !top bits discarded!
C_t one_stage_mult_add(A_t a, B_t b, C_t in) {
    cascade<REG_A1 | REG_B1 | REG_M | REG_P> cas;
    R_t res = cas.mul_add(a, b, in);
    return res.pcout;
}

void transposed_fir( long &data_out, long data_in,
             long h0, long h1, long h2, long h3) {
    #pragma HLS pipeline II=1
    C_t pc_0_1, pc_1_2, pc_2_3, p_3;
    A_t data_in_fanout = data_in;
    pc_0_1 = one_stage_mult_add( h0, data_in_fanout, 0); // mul only so adds 0
    pc_1_2 = one_stage_mult_add( h1, data_in_fanout, pc_0_1);
    pc_2_3 = one_stage_mult_add( h2, data_in_fanout, pc_1_2);
    p_3    = one_stage_mult_add( h3, data_in_fanout, pc_2_3);
    data_out = p_3;
}
