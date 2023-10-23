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
    hls::stream<int> a;
    hls::stream<ap_uint<1>> e;
    hls::stream<ap_uint<32>> b1;
    hls::stream<ap_uint<32>> b2;
    hls::stream<ap_uint<32>> b3;
    hls::stream<ap_uint<32>> b4;
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    int cnt4 = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            a.write(j);
        }
    }

    dut(a, b1, b2, b3, b4);

    for (int i = 0; i < 4; i++) {
        int tmp1 = b1.read();
        if (tmp1 == 0)
            cnt1 += 1;
        else if (tmp1 == 1)
            cnt2 += 1;
        else if (tmp1 == 2)
            cnt3 += 1;
        else if (tmp1 == 3)
            cnt4 += 1;
        int tmp2 = b2.read();
        if (tmp2 == 0)
            cnt1 += 1;
        else if (tmp2 == 1)
            cnt2 += 1;
        else if (tmp2 == 2)
            cnt3 += 1;
        else if (tmp2 == 3)
            cnt4 += 1;
        int tmp3 = b3.read();
        if (tmp3 == 0)
            cnt1 += 1;
        else if (tmp3 == 1)
            cnt2 += 1;
        else if (tmp3 == 2)
            cnt3 += 1;
        else if (tmp3 == 3)
            cnt4 += 1;
        int tmp4 = b4.read();
        if (tmp4 == 0)
            cnt1 += 1;
        else if (tmp4 == 1)
            cnt2 += 1;
        else if (tmp4 == 2)
            cnt3 += 1;
        else if (tmp4 == 3)
            cnt4 += 1;
    }

    if (cnt1 != 4 || cnt2 != 4 || cnt3 != 4 || cnt4 != 4)
        return 1;

    return 0;
}
