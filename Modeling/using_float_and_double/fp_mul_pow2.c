/*
 * Copyright 2022 Xilinx, Inc.
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

#include "fp_mul_pow2.h"

#ifdef ABS
#undef ABS
#endif
#define ABS(n) ((n < 0) ? -n : n)

float float_mul_pow2(float x, int8_t n) {
  float_num_t x_num, prod;

  x_num.fp_num = x;
#ifndef AESL_FP_MATH_NO_BOUNDS_TESTS
  if (x_num.bexp == 0xFF || x_num.bexp == 0) // pass through NaN, INF & denorm
    prod.fp_num = x_num.fp_num;
  else if (n >= 0 && x_num.bexp >= 255 - n) { // detect and handle overflow
    prod.sign = x_num.sign;                   //
    prod.bexp = 0xFF;                         // +/-INF
    prod.mant = 0;                            //
  } else if (n < 0 &&
             x_num.bexp <= ABS(n)) { // handle underflow (doesn't gen denorms)
    prod.sign = x_num.sign;          //
    prod.bexp = 0;                   // +/-ZERO
    prod.mant = 0;                   //
  } else
#endif // AESL_FP_MATH_NO_BOUNDS_TESTS not defined
  {
    prod.sign = x_num.sign;
    prod.bexp = x_num.bexp + n;
    prod.mant = x_num.mant;
  }
  return prod.fp_num;
}

double double_mul_pow2(double x, int16_t n) {
  double_num_t x_num, prod;

  x_num.fp_num = x;
#ifndef AESL_FP_MATH_NO_BOUNDS_TESTS
  if (x_num.bexp == 0xFF || x_num.bexp == 0) // pass through NaN, INF & denorm
    prod.fp_num = x_num.fp_num;
  else if (n >= 0 && x_num.bexp >= 2047 - n) { // detect and handle overflow
    prod.sign = x_num.sign;                    //
    prod.bexp = 0x7FF;                         // +/-INF
    prod.mant = 0;                             //
  } else if (n < 0 &&
             x_num.bexp <= ABS(n)) { // handle underflow (doesn't gen denorms)
    prod.sign = x_num.sign;          //
    prod.bexp = 0;                   // +/-ZERO
    prod.mant = 0;                   //
  } else
#endif // AESL_FP_MATH_NO_BOUNDS_TESTS not defined
  {
    prod.sign = x_num.sign;
    prod.bexp = x_num.bexp + n;
    prod.mant = x_num.mant;
  }
  return prod.fp_num;
}
