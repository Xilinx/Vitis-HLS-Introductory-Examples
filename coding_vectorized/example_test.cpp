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

#include <vector>
#include <iostream>

typedef float float16 __attribute__((vector_size(64)));

extern "C"
void example(float16*res, const float16 *lhs, const float16 *rhs, int n);

int main(int, char**) {
  std::vector<float16> lhs(1000);
  std::vector<float16> rhs(1000);
  std::vector<float16> res(1000);

  float16 temp = {3.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 3.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1};
 
  lhs[1] = temp; 
  rhs[1] = temp;

  example(res.data(), lhs.data(), rhs.data(), 4);

  std::cout << "The output value for res[1][1] should be 2.2 and is " << res[1][1] << std::endl;

  return 0;
}