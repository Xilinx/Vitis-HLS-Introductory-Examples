#include <hls_dsp_builtins.h>
using namespace hls::dsp58;
#define N 10

const int TAPS=4;

void systolic_fir( long a[TAPS], long b[N], long c, long hw[N]);

