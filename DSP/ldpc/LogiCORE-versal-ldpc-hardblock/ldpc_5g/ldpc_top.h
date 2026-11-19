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
#include "hls_versal_ldpc.h"

using namespace hls::versal_ldpc;

/***************************************************************
 Static configuration of the Versal LDPC Decoder hard IP (hls_versal_ldpc.h)
 The parameters have the names and default values of the IP customization in Vivado.
    Standard                      STD_5G, STD_DVBS2X (default), STD_WIFI, STD_DOCSIS or STD_CUSTOM
    DIN_Lanes, DOUT_Lanes         1 or 2 lanes of 256 bits (default: 1)
    DIN_Words_Configuration,      WORDS_FIXED (default), WORDS_PER_BLOCK or WORDS_PER_TRANSACTION
    DOUT_Words_Configuration      (the words streams are arguments of ldpc_decode when not fixed)
    LDPC_Decode_Algorithm, ...    Decoding algorithm, modifier, saturation bits and maximum
                                  schedule (see the product guide PG452)
 Only the parameters different from the default values are set here.
 For the 5G standard, the code is selected for each block by the geometry fields
 of the control packet (lifting size, base graph and number of parity rows),
 see ldpc_tb.cpp.
****************************************************************/
struct config : params_t {
    static const unsigned Standard = STD_5G;
};

// The four AXI4-Stream channels of the decoder with fixed words configuration
void ldpc_top(hls::stream<ctrl_t>&           ctrl_s,
              hls::stream<din_bus<config>>&  din_s,
              hls::stream<dout_bus<config>>& dout_s,
              hls::stream<status_t>&         status_s);
