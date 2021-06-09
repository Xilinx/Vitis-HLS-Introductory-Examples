/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "filter_scalar.h"

void filter(data_t &x, coef_t coef[TAP], sum_t &y)
{
#pragma HLS INTERFACE ap_fifo port=x
#pragma HLS INTERFACE ap_fifo port=y
#pragma HLS INTERFACE ap_fifo port=coef
#pragma HLS PIPELINE II=4

  static systolic<data_t, coef_t, sum_t> s[TAP];
  coef_t h[TAP];
  data_t x_int[TAP];
  sum_t y_int[TAP];
  sum_t tmp = 0;

  // Copy, partition the coefficients to make them available in
  // parallel.
#pragma HLS ARRAY_PARTITION variable = h complete dim = 1
  for (int i = 0; i < TAP; i++) {
    h[i] = coef[i];
  }

  // Connect the filter execution blocks to implement the systolic
  // filter chain.
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
