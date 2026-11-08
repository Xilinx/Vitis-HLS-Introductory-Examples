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
#ifndef __AUTORESTART_PATTERN1_TEST_H__
#define __AUTORESTART_PATTERN1_TEST_H__

#include "hls_top.h"
#include "hls_directio.h"
#include "hls_stream.h"

const int N = 16;

HLS_TOP_AUTORESTART(ar_inf_stream, hls::stream<int> &sin,
                    hls::stream<int> &sout, int &c);

#endif
