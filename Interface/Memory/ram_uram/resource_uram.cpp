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

#include "resource_uram.h"

void resource_uram(bool wren, bool rden, addr_t addrW, data_t datain,
                   addr_t AddrR, data_t* dataout) {
#pragma HLS PIPELINE II = 1

    static data_t buffer[NWORDS];
#pragma HLS DEPENDENCE variable = buffer inter WAR false
#pragma HLS BIND_STORAGE variable = buffer type = ram_2p impl = uram

    if (rden)
        *dataout = buffer[AddrR];

    if (wren)
        buffer[addrW] = datain;
}
