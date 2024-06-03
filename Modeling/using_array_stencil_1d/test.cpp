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
#include<math.h>
#include<string.h>
#include<iostream>
#include<math.h>
#define HS 100
#define WS 100
#define FW 3
#define FH 1

void top(float in[HS][WS], float out[HS][WS-FW+1], float var_in);

void top_sw(float in[HS][WS], float out[HS][WS-FW+1], float var_in) {
   for (int i = 0; i < 100; i++) {
     for (int j = 0; j < 100-FW+1; j++) {
        out[i][j] = in[i][j] + in[i][j+1] + in[i][j+2] + var_in;
     }
   }
}

int main() {
  float in[HS][WS];
  float out[HS][WS -FW + 1];
  float out_sw[HS][WS -FW + 1];
  for (int i = 0; i < HS; ++i)
   for (int j = 0; j < WS; ++j)
      in[i][j] = j+i;
  float var = 10.2;
  top(in, out, var);
  top_sw(in, out_sw, var);
  int res = 0;
  for (int i = 0; i < HS; ++i)
     for (int j = 0; j < 100-FW+1; j++)
      if (fabs((fabs(out[i][j]) + 0.01) / (fabs(out_sw[i][j]) + 0.01) - 1) > 0.001) {
        res = 1;
        std::cout << "(" << i << "," << j << "): " << out[i][j] << " vs " << out_sw[i][j] << '\n';
      }
  return res;
}
