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

#ifndef _LOOP_PERFECT_H_
#define _LOOP_PERFECT_H_

#include <fstream>
#include <iostream>
using namespace std;

#include "ap_int.h"
#define N 20

typedef ap_int<5> din_t;
typedef ap_int<12> dint_t;
typedef ap_int<6> dout_t;

void loop_perfect(din_t A[N], dout_t B[N]);

#endif
