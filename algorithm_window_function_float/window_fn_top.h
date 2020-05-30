/*
 * Copyright 2020 Xilinx, Inc.
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

#ifndef WINDOW_FN_TOP_H_
#define WINDOW_FN_TOP_H_

#include "window_fn_class.h"

// Test parameters
#define FLOAT_DATA // Used to select error tolerance in test program
#define WIN_TYPE xhls_window_fn::HANN
#define WIN_LEN 32

// Define floating point types for input, output and window coefficients
typedef float win_fn_in_t;
typedef float win_fn_out_t;
typedef float win_fn_coef_t;

// Top level function prototype - wraps all object, method and functions for HLS
void window_fn_top(win_fn_out_t outdata[WIN_LEN], win_fn_in_t indata[WIN_LEN]);

#endif // WINDOW_FN_TOP_H_

