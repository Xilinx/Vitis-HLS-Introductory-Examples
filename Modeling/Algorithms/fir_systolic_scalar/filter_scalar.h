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

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#if 0
typedef int data_t;
typedef int coef_t;
typedef int sum_t;
#else
#include "ap_int.h"
typedef ap_int<16> data_t;
typedef ap_int<16> coef_t;
typedef ap_int<34> sum_t;
#endif

const int TAP = 4;

void filter(data_t &x, coef_t h[TAP], sum_t &y);

// Filtering unit cell class
template <typename T0, typename T1, typename T2>
class systolic
{
private:
  T0 x,  x1;
  T2 y0, y1;

public:
  // Based on systolic fir description in Xilinx UG073
  void exec(T0 &x_in, T1 &h, T2 &y_in, T0 &x_out, T2 &y)
  {
    x_out = x1;
    x1    = x;
    x     = x_in;
    y     = y1;
    y1    = y0 + y_in;
    y0    = x_out * h;
  }
};
