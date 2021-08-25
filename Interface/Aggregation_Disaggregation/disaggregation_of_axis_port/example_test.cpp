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
#include "example.h"

int main() {
  A arr[N], out[N];
  for (unsigned i=0; i<N; i++)
    arr[i].c = arr[i].i = i;

  dut(arr, out);
  for (unsigned i=0; i<N; i++) {
    std::cout << i<< ": " << int(out[i].c) << ", " << out[i].i << std::endl;
    if (out[i].c != i || out[i].i != i)
      return 1;
  }
  return 0;
}
