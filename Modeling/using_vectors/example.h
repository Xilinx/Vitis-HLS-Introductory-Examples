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
#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include "hls_vector.h"
#include <ap_int.h>

#define NUM 32

// Each vector will be 64 bytes (16 x 4 bytes)
typedef hls::vector<float, 16> float16;

extern "C" void example(float16* res, const float16* lhs, const float16* rhs,
                        int n);

#endif
