/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "example.h"

int main() {
    hls::stream<int> s_in[M], s_out[M];
    for (unsigned j = 0; j < M; j++) {
        for (unsigned i = 0; i < 10; i++) {
            s_in[j].write(i);
        }
    }

    auto ret = dut(s_in, s_out);

    std::cout << "ret = " << ret << std::endl;

    for (unsigned j = 0; j < M; j++) {
        for (unsigned i = 0; i < 10; i++) {
            if (s_out[j].read() != i + 2)
                return 1;
        }
    }

    return 0;
}
