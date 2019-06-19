#if 0
typedef int data_t;
typedef int coef_t;
typedef int sum_t;
#else
#include "ap_int.h"
typedef ap_int<12> data_t;
typedef ap_int<12> coef_t;
typedef ap_int<24> sum_t;
#endif

const int TAP = 4;

// Original engine
template <typename T0, typename T1, typename T2>
class systolic
{
private:
  T0 x;
  T0 x1;
  T2 y0;
  T2 y1;

public:
// Initial version
  void exec(T0 &x_in, T1 &h, T2 &y_in, T0 &x_out, T2 &y)
  {
    y     = y1;
    x_out = x;
    y1    = y0;
    y0    = x * h + y_in;
    x     = x_in;
  }
  // Based on systolic fir description in Xilinx UG073
  void exec073(T0 &x_in, T1 &h, T2 &y_in, T0 &x_out, T2 &y)
  {
    x_out = x1;
    x1    = x;
    x     = x_in;
    y     = y1;
    y1    = y0 + y_in;
    y0    = x_out * h;
  }
};
