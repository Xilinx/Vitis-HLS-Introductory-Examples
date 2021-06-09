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

#include "lookup.h"

dout_t lookup(din1_t inval, din2_t idx)
{
   din1_t lookup_table[256];
   dint_t i;

   for (i = 0; i < 256; i++) {
      lookup_table[i] = 256 * (i - 128);
   }

   return (dout_t)inval * (dout_t)lookup_table[idx];
}
