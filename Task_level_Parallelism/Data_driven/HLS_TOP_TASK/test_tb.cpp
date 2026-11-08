/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include <cstdio>

int main() {
    hls::ap_hs<int> in;
    hls::ap_ack<int> out;
    int c = 9;

    for (int i = 0; i < N; i++)
        in.write(i);

    tt_p_hs_ack t;
    t.launch(in, out, c);

    for (int i = 0; i < N; i++) {
        int v = out.read();
        if (v != i + c) {
            printf("ERROR i=%d got=%d exp=%d\n", i, v, i + c);
            return 1;
        }
    }
    printf("PASS\n");
    return 0;
}
