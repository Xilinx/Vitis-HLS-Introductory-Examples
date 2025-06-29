/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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

#ifndef _ARRAY_MEM_BOTTLENECK_H_
#define _ARRAY_MEM_BOTTLENECK_H_

#include <fstream>
#include <iostream>
using namespace std;

#include "ap_int.h"
#define N 128

typedef ap_int<7> din_t;
typedef ap_int<10> dout_t;

dout_t array_mem_bottleneck(din_t mem[N]);

#endif
