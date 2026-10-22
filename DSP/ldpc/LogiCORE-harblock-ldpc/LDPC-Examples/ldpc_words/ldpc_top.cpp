/*
 * Copyright 2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "ldpc_top.h"

void ldpc_top(hls::stream<ctrl_t>&               ctrl_s,
              hls::stream<din_bus<config>>&      din_s,
              hls::stream<din_words_t<config>>&  din_words_s,
              hls::stream<dout_bus<config>>&     dout_s,
              hls::stream<dout_words_t<config>>& dout_words_s,
              hls::stream<status_t>&             status_s)
{
    // The call of ldpc_decode is the only process of the design. Each call
    // decodes one block.
    ldpc_decode<config>(ctrl_s, din_s, din_words_s, dout_s, dout_words_s, status_s);
}
