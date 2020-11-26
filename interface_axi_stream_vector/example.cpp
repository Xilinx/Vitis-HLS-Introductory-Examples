/*
 * Copyright 2020 Xilinx, Inc.
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

void example(hls::stream<float4> &a, hls::stream<float4> &b)
{
#pragma HLS INTERFACE axis register_mode=both register port=b
#pragma HLS INTERFACE axis register_mode=both register port=a

  float4 sample; // float4 is defined as a vector type (4 float)

  while (!a.empty())
  {
    a >> sample;
    b << sample * 1.8 + 32;  // Celsius to Fahrenheit formula
  }
}
