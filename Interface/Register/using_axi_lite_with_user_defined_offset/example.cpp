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

#include <stdio.h>
// In this example the user uses to define the offset and the range is dependent
// on the datatype and port protocol it usese. See the saxi lite  documentation
// which explains the example in more detail

void example(char* a, char* b, char* c) {
#pragma HLS INTERFACE s_axilite port = a bundle = BUS_A offset = 0x20
#pragma HLS INTERFACE s_axilite port = b bundle = BUS_A offset = 0x28
#pragma HLS INTERFACE s_axilite port = c bundle = BUS_A offset = 0x30
#pragma HLS INTERFACE s_axilite port = return bundle = BUS_A

    *c += *a + *b;
}
