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

#ifndef WINDOW_FN_TOP_H_
#define WINDOW_FN_TOP_H_

#include "window_fn_class.h"

// Test parameters
#define WIN_TYPE xhls_window_fn::HANN
#define WIN_LEN 32

// Types and top-level function prototype
#include <ap_int.h>
// Define widths of fixed point fields
#define W_IN    8
#define IW_IN   8
#define W_OUT   24
#define IW_OUT  8
#define W_COEF  18
#define IW_COEF 2

// Define fixed  point types for input, output and coefficients
typedef ap_fixed<W_IN,IW_IN> win_fn_in_t;
typedef ap_fixed<W_OUT,IW_OUT> win_fn_out_t;
typedef ap_fixed<W_COEF,IW_COEF> win_fn_coef_t;

// Top level function prototype - wraps all object, method and functions for HLS
void window_fn_top(win_fn_out_t outdata[WIN_LEN], win_fn_in_t indata[WIN_LEN]);

#endif // WINDOW_FN_TOP_H_

