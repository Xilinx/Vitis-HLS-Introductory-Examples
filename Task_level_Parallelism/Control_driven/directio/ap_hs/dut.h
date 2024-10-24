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
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "hls_directio.h"
#include <iostream>
#define DWIDTH 32

typedef ap_axiu<DWIDTH, 2, 5, 6> pkt;
typedef ap_axiu<DWIDTH, 2, 5, 6> packet;

#define threshold 5000
#define DATA 12000
void krnl_stream_vdatamover(hls::stream<pkt> &in,
                      hls::stream<pkt> &out,
					  int *mem,
                      hls::ap_hs<int> &reset_value,
                      hls::ap_hs<int> &reset_myCounter
                      );