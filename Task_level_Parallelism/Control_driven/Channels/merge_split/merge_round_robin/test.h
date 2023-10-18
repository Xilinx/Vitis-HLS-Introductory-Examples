#ifndef __MERGE_ROUND_ROBIN_HH__
#define __MERGE_ROUND_ROBIN_HH__
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
#include "hls_np_channel.h"
#include "hls_stream.h"

void dut(hls::stream<int>& in1, hls::stream<int>& out1, hls::stream<int>& out2,
         hls::stream<int>& out3, hls::stream<int>& out4);

#endif
