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
    static TestStruct<10> ts[2] = {{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                    {9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
                                    {8, 8, 7, 7, 5, 4, 3, 2, 1, 0}},
                                   {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                    {9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
                                    {9, 9, 8, 6, 5, 4, 3, 2, 1, 0}}};

#pragma HLS BIND_STORAGE variable = ts type = RAM_2P impl = BRAM
    // #pragma HLS BIND_STORAGE variable=ts type=RAM_2P impl=LUTRAM
    // URAMs are not supported for global/static arrays
    // #pragma HLS BIND_STORAGE variable=ts type=RAM_2P impl=URAM

    int ind = i % 2;
    ts[ind].A[i] += ts[ind].B[i] + ts[ind].C[i];
    ts[ind].B[i] += 5;
    ts[ind].C[i] += 10;

    int result = (ts[ind].A[i] + ts[ind].B[i] + ts[ind].C[i]);
    return result;
}
