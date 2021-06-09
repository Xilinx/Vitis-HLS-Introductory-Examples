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

#include <stdio.h>

void example_no_label(int A[50], int B[50]); 
void example_label(int A[50], int B[50]);

int main()
{
  int i;
  int A[50];
  int B[50];
  int C[50];

  for(i=0; i < 50; i++){
    A[i] = i;
  }

  example_no_label(A,B);
  example_label(A,C);

  for(i=0; i < 50; i++){
    if(B[i] != C[i]){
      printf("ERROR: example_label and example_no_label mismatch\n");
      return 1;
    }
  }

  printf("Success: results from both functions match\n");
  return 0;
}

