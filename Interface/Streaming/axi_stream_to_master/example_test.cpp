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

int main() {
    int err = 0;
    ap_uint<64> out_t[1024];
    hls::stream<trans_pkt> inStream_t;
    trans_pkt dataStream_t;

    for (int i = 0; i < 1024; i++) {
        dataStream_t.data = i;
        dataStream_t.keep = -1;
        if (i < 1023) {
            dataStream_t.last = 0;
        } else {
            dataStream_t.last = 1;
        }
        inStream_t.write(dataStream_t);
    }

    example(inStream_t, out_t);
    return err;
}
