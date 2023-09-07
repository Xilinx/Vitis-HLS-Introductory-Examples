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

#ifndef _LOOKUP_MATH_H_
#define _LOOKUP_MATH_H_

#include <fstream>
#include <iostream>
using namespace std;

typedef short din1_t;
typedef unsigned char din2_t;
typedef double dint_t;
typedef int dout_t;

#define M_PI 3.14159265358979323846

dout_t lookup_math(din1_t inval, din2_t idx);

#endif
