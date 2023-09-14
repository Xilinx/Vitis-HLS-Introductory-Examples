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

#include "pointer_arith.h"

int main() {
  dio_t d[5], ref[5];
  int i, retval = 0;
  FILE *fp;

  // Create input data
  for (i = 0; i < 5; i++) {
    d[i] = i;
    ref[i] = i;
  }

  // Call the function to operate on the data
  pointer_arith(d);

  // Save the results to a file
  fp = fopen("result.dat", "w");
  printf(" Din Dout\n");
  for (i = 0; i < 4; i++) {
    fprintf(fp, "%d \n", d[i]);
    printf("  %d   %d\n", ref[i], d[i]);
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
