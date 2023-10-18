/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
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

#include "example.h"

int main(int, char**) {
    S a[N] = {{0, {5, 1, 0}}, {1, {5, 2, 1}}, {2, {5, 3, 1}},
              {3, {5, 4, 0}}, {4, {5, 5, 1}}, {5, {5, 6, 0}}};
    S b[N] = {{9, {5, 10, 1}},  {10, {5, 11, 1}}, {11, {5, 12, 0}},
              {12, {5, 13, 0}}, {13, {5, 14, 1}}, {14, {5, 15, 1}}};
    S c[N];

    top(a, b, c);

    if ((c[4].q.m == 10) && (c[5].q.n == -9))
        std::cout << "PASSED " << c[4].q.m << "," << c[5].q.n << std::endl;
    else
        std::cout << "FAILED " << c[4].q.m << "," << c[5].q.n << std::endl;

    return 0;
}
