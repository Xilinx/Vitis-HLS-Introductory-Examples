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
#include "using_ap_float_accumulator.h"

void using_ap_float_accumulator( stream_t &instream, stream_t &outstream, int n) {
    apf_t res;
    ap_float_acc<apf_t, acc_datatype_t> acc;
accloop:
    for (unsigned int i = 0; i < n; i++) {
        apf_t value = instream.read();
        res = acc.accumulate(value, i == n-1);
    }
    outstream.write((float)res);
}

