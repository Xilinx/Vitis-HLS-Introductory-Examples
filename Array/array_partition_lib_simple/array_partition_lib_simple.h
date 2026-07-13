/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#ifndef __ARRAY_PARTITION_LIB_SIMPLE_H__
#define __ARRAY_PARTITION_LIB_SIMPLE_H__

#include "hls_array_partition.h"
#include "hls_vector.h"

#include <cstddef>

#define ARRAY_SIZE 40
#define K 4

float array_partition_lib_simple(float A[ARRAY_SIZE], size_t i, size_t s);

#endif // __ARRAY_PARTITION_LIB_SIMPLE_H__
