#include "filter_frame.h"

//-----------------------------------------
void run(data_t x, coef_t h[TAP], sum_t &y)
{
#pragma HLS INLINE off

  static systolic<data_t, coef_t, sum_t> s[TAP];

  data_t x_int[TAP];
  sum_t y_int[TAP];
  sum_t tmp = 0;

/* code */
connect:
  for (int i = 0; i < TAP; i++)
  {
    if (i == 0)
      s[0].exec(x, h[i], tmp, x_int[i], y_int[i]);
    else
      s[i].exec(x_int[i - 1], h[i], y_int[i - 1], x_int[i], y_int[i]);
    y = y_int[TAP - 1];
  }
}

//-----------------------------------------------
void filter(data_t x[FRM], coef_t h[TAP], sum_t &y)
{
#pragma HLS INTERFACE ap_fifo depth=FRM port=y
#pragma HLS ARRAY_PARTITION variable=h complete dim=1
#pragma HLS PIPELINE

unpack:
  for (int k = 0; k < FRM; k++)
    run(x[k], h, y);
}
