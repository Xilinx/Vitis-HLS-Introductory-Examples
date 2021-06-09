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
#include <iomanip>
using namespace std;

#include "window_fn_top.h"

#ifdef FLOAT_DATA
#define ABS_ERR_THRESH 0.0
#else
#define ABS_ERR_THRESH 0.001
#endif

#define WINDOW_FN_DEBUG 1

int main(void)
{
   win_fn_in_t testdata[WIN_LEN];
   win_fn_out_t hw_result[WIN_LEN];
   float sw_result[WIN_LEN];
   unsigned err_cnt = 0;

   // Generate test vectors and expected (real valued) results
   for (int i = 0; i < WIN_LEN; i++) {
      float samp_i = 64.0;
      // Use the coefficient calculation function from class for convenience
      float win_i = xhls_window_fn::coef_calc<WIN_LEN,WIN_TYPE>(i);
      testdata[i] = samp_i; // implicit conversion from float to ap_fixed<>
      sw_result[i] = samp_i * win_i; // generate floating point expected values
   }

   // Run the DUT
   window_fn_top(hw_result, testdata);

   // Check results
   cout << "Checking results against a tolerance of " << ABS_ERR_THRESH << endl;
   cout << fixed << setprecision(5);
   for (unsigned i = 0; i < WIN_LEN; i++) {
      float abs_err = float(hw_result[i]) - sw_result[i];
#if WINDOW_FN_DEBUG
      cout << "i = " << i << "\thw_result = " << hw_result[i];
      cout << "\t sw_result = " << sw_result[i] << endl;
#endif
      if (fabs(abs_err) > ABS_ERR_THRESH) {
         cout << "Error threshold exceeded: i = " << i;
         cout << "  Expected: " << sw_result[i];
         cout << "  Got: " << hw_result[i];
         cout << "  Delta: " << abs_err << endl;
         err_cnt++;
      }
   }
   cout << endl;

   // Print final status message
   if (err_cnt) {
      cout << "!!! TEST FAILED - " << err_cnt;
      cout << " results out of tolerance." << endl;
   } else
      cout << "Test Passed" << endl;

   // Only return 0 on success
   if (err_cnt)
       return 1;
   else
       return 0;
}


