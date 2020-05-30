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

#include "window_fn_top.h" // Provides typedefs and params

// Include the entire xhls_window_fn namespace so that scope resolution --
// i.e. prepending xhls_window_fn:: to everything -- is not necessary
using namespace xhls_window_fn;

//Vivado HLSrequires a top-level function definition that wraps all object
// instantiations and method calls to be synthesized as well as mapping
// the top-level I/O (function arguments) into/out of the methods/functions.
void window_fn_top(
      win_fn_out_t outdata[WIN_LEN],
      win_fn_in_t indata[WIN_LEN])
{
   // Instatiate a window_fn object - types and params defined in header file
   window_fn<win_fn_in_t,win_fn_out_t,win_fn_coef_t,
      WIN_LEN,WIN_TYPE> my_win_fn;

   // Call the apply() method - an input data set is windowed for each "call"
   // to window_fn_top()
   my_win_fn.apply(outdata, indata);
}


