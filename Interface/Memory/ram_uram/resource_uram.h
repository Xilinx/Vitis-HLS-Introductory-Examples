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

#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

#define ADDRBITS 14
#define NWORDS 1 << ADDRBITS

// Switch between regular and hw data types
#if 0
typedef int data_t;
typedef int addr_t;
#else
#include "ap_int.h"
#define DATAWIDTH 128
typedef ap_uint<DATAWIDTH> data_t;
typedef ap_uint<ADDRBITS> addr_t;
#endif

void resource_uram(bool wren, bool rden, addr_t addrW, data_t datain,
                   addr_t AddrR, data_t* dataout);
