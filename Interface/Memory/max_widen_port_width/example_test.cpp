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
using namespace std;

int main() {
    int in[N], res[N];
    for (size_t i = 0; i < N; ++i)
        in[i] = i;

    example(in, res);

    size_t err = 0;
    for (int i = 0; i < N; ++i)
        if (res[i] != i + 100)
            return EXIT_FAILURE;

    cout << "Test passed.\n";
    return EXIT_SUCCESS;
}
