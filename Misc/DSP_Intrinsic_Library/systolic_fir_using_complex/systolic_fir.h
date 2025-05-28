#include <hls_dsp_builtins.h>

using namespace hls::dspcplx;

const int NDATA=10;
const int TAPS=4;

void systolic_fir( A_t coeff[TAPS], B_t b[NDATA], C_t bias, C_t hw[NDATA]);
