/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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

void add_kernel(int tmp1[128], int tmp2[128], int tmp3[128]) {

    for (int i = 0; i < 128; i++) {
        tmp3[i] = tmp1[i] + tmp2[i];
    }
}

void Double_pass(int tmp2[128], int tmp1[128], int tmp4[128], int tmp5[128]) {

    for (int i = 0; i < 128; i++) {
        tmp4[i] = tmp1[i];
        tmp5[i] = tmp2[i];
    }
}

void pass(int a[128], int b[128], int tmp1[128], int tmp2[128]) {

    for (int i = 0; i < 128; i++) {
        tmp1[i] = a[i];
        tmp2[i] = b[i];
    }
}

void dut(int a[128], int b[128], int tmp3[128]) {
#pragma HLS DATAFLOW

    int tmp1[128], tmp2[128], tmp4[128];
    int tmp5[128];

    pass(a, b, tmp1, tmp2);
    Double_pass(tmp2, tmp1, tmp4, tmp5);
    add_kernel(tmp4, tmp5, tmp3);
}
