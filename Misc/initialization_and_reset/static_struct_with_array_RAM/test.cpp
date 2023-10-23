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

int test(int i) {
    static TestStruct<10> ts = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                {9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
                                {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};

#pragma HLS BIND_STORAGE variable = ts.A type = RAM_2P impl = BRAM
#pragma HLS BIND_STORAGE variable = ts.B type = RAM_2P impl = LUTRAM

    // URAM on non-Versal devices cannot be initialized
    // #pragma HLS BIND_STORAGE variable=ts.C type=RAM_2P impl=URAM

    ts.A[i] += ts.B[i] + ts.C[i];
    ts.B[i] += 5;
    ts.C[i] += 10;

    return (ts.A[i] + ts.B[i] + ts.C[i]);
}
