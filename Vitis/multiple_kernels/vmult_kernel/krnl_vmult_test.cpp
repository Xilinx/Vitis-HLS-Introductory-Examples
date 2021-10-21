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

#include "krnl_vmult.hpp"

int main() {
    
  uint32_t in1[DATA_SIZE], in2[DATA_SIZE];
  uint32_t out[DATA_SIZE], res[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i){
    in1[i] = i;
    in2[i] = i;
    out[i] = 0;
    res[i] = in1[i] * in2[i];
  }

  krnl_vmult(in1, in2, out, DATA_SIZE);

  for (int i = 0; i < DATA_SIZE; ++i){
    if (res[i] != out[i])
      return EXIT_FAILURE;
    }

  std::cout << "Test passed.\n";
  return EXIT_SUCCESS;
}
