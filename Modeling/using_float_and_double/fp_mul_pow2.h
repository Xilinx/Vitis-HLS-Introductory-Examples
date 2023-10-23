/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
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

#ifndef FLOAT_MUL_POW2_H_
#define FLOAT_MUL_POW2_H_

#include <stdint.h>

// Uncomment the following line to eliminate bounds checking, ignoring
// overlow, underflow, NaN inputs, etc.
//#define AESL_FP_MATH_NO_BOUNDS_TESTS

// Helper typedefs that allow easy parsing of the IEEE-754 floating point
// format fields (sign, biased exponent and mantissa), via C bit-fields, as
// well as directly manipulating the whole word bitfield ('raw_bits').
typedef union {
    float fp_num;
    uint32_t raw_bits;
    struct {
        uint32_t mant : 23;
        uint32_t bexp : 8;
        uint32_t sign : 1;
    };
} float_num_t;

typedef union {
    double fp_num;
    uint64_t raw_bits;
    struct {
        uint64_t mant : 52;
        uint64_t bexp : 11;
        uint64_t sign : 1;
    };
} double_num_t;

// These functions implement floating point (single- and double-precision)
// multiplication by a power-of-two for HLS. Multiplication by a power-of-two
// can be much more effecient that arbitrary multiplitcation, because it can
// be reduced to a simple 8- or 11-bit (single- & double-precision
// respectively) addition to the biased exponent w/ some basic checks for
// overflow and underflow (which can be eliminated if desired by defining the
// preprocessor macro AESL_FP_MATH_NO_BOUNDS_TESTS.
float float_mul_pow2(float x, int8_t n);
double double_mul_pow2(double x, int16_t n);

#endif // FLOAT_MUL_POW2_H_
