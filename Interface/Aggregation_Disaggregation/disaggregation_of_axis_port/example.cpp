/*
 * Copyright 2021 Xilinx, Inc.
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

#include "example.h"

void dut(A in[N], A out[N]) {
#pragma HLS interface axis port=in
#pragma HLS interface axis port=out
#pragma HLS disaggregate variable=in
#pragma HLS disaggregate variable=out
  int sum = 0;
  for (unsigned i=0; i<N; i++) {
    out[i].c = in[i].c;
    out[i].i = in[i].i;
  }
}
