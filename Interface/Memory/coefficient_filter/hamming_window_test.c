/*
 * Copyright 2022 AMD, Inc.
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
#include <stdio.h>

#include "hamming_window.h"

int main(int argc, char *argv[]) {
  int16_t test_data[WINDOW_LEN];
  int32_t hw_result[WINDOW_LEN], sw_result[WINDOW_LEN];
  int i;
  unsigned err_cnt = 0, check_dots = 0;

  for (i = 0; i < WINDOW_LEN; i++) {
    // Generate a test pattern for input to DUT
    test_data[i] = (int16_t)((32767.0 * (double)((i % 16) - 8) / 8.0) + 0.5);
    // Calculate the coefficient value for this index
    int16_t coeff_val = (int16_t)(round(
        WIN_COEFF_SCALE *
        (0.54 - 0.46 * cos(2.0 * M_PI * i / (double)(WINDOW_LEN - 1)))));
    // Generate array of expected values -- n.b. explicit casts to avoid
    // integer promotion issues
    sw_result[i] = (int32_t)test_data[i] * (int32_t)coeff_val;
  }

  // Call the DUT
  printf("Running DUT...");
  hamming_window(hw_result, test_data);
  printf("done.\n");

  // Check the results returned by DUT against expected values
  printf("Testing DUT results");
  for (i = 0; i < WINDOW_LEN; i++) {
    if (hw_result[i] != sw_result[i]) {
      err_cnt++;
      check_dots = 0;
      printf("\n!!! ERROR at i = %4d - expected: %10d\tgot: %10d", i,
             sw_result[i], hw_result[i]);
    } else { // indicate progress on console
      if (check_dots == 0)
        printf("\n");
      printf(".");
      if (++check_dots == 64)
        check_dots = 0;
    }
  }
  printf("\n");

  // Print final status message
  if (err_cnt) {
    printf("!!! TEST FAILED - %d errors detected !!!\n", err_cnt);
  } else
    printf("*** Test Passed ***\n");

  // Only return 0 on success
  if (err_cnt)
    return 1;
  else
    return 0;
}
