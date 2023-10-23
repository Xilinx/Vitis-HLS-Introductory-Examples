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

#ifndef _CPP_AP_FIXED_H_
#define _CPP_AP_FIXED_H_

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;
#include "ap_fixed.h"

typedef ap_ufixed<10, 8, AP_RND, AP_SAT> din1_t;
typedef ap_fixed<6, 3, AP_RND, AP_WRAP> din2_t;
typedef ap_fixed<22, 17, AP_TRN, AP_SAT> dint_t;
typedef ap_fixed<36, 30> dout_t;

dout_t cpp_ap_fixed(din1_t d_in1, din2_t d_in2);

#endif
