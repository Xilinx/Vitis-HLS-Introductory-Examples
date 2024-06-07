/*
 * Copyright 2023 Xilinx, Inc.
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

#include "pointer_stream_good.h"

int main() {
  din_t d_i[4];
  dout_t d_o[4];
  int i, retval = 0;
  FILE *fp;

  // Create input data
  for (i = 0; i < 4; i++) {
    d_i[i] = i;
  }

  // Call the function to operate on the data
  pointer_stream_good(d_o, d_i);

  // Save the results to a file
  fp = fopen("result.dat", "w");
  fprintf(fp, "Din Dout\n");
  for (i = 0; i < 4; i++) {
    if (i < 2)
      fprintf(fp, "%d   %d\n", d_i[i], d_o[i]);
    else
      fprintf(fp, "%d \n", d_i[i]);
  }
  fclose(fp);

  // Compare the results file with the golden results
  retval = system("diff --brief -w result.dat result.golden.dat");
  if (retval != 0) {
    printf("Test failed  !!!\n");
    retval = 1;
  } else {
    printf("Test passed !\n");
  }

  // Return 0 if the test passed
  return retval;
}
