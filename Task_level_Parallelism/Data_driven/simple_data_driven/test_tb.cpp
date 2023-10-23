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
#include "test.h"

int main() {

    hls::stream<int, N> in;
    hls::stream<int, N / 2> out1;
    hls::stream<int, N / 2> out2;

    for (int i = 0; i < N; i++)
        in.write(i);

    odds_and_evens(in, out1, out2);

    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < N / 2; i++)
        sum1 += out1.read();
    for (int i = 0; i < N / 2; i++)
        sum2 += out2.read();

    if ((sum1 != 30) && (sum2 != 30))
        return 1;

    return 0;
}
