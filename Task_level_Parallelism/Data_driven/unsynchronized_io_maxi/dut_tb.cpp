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
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <unistd.h>

extern "C" void stable_pointer(int* mem, hls::stream<int>&, hls::stream<int>&);

int main(int argc, char* argv[]) {
    int memory[512];
    // memset(memory, 0, 128*256/8);
    hls::stream<int> in, out;
    int val;
    for (int i = 0; i < 100; i++) {
        in.write(i);
        memory[i] = 0;
    }

    stable_pointer(memory, in, out);

    for (int i = 0; i < 100; i++) {
        out.read(val);
        if (val != i * 23 * 11) {
            printf("%d != %d\n", val, i * 23 * 11);
            return 1;
        }
    }

    return 0;
}
