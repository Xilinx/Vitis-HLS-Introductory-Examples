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
 (see the ldpc_5g example for the description of the parameters)
   Standard                   STD_5G: the code is selected for each block by the
                              geometry fields of the control packet
   DIN_Words_Configuration    WORDS_PER_TRANSACTION: the number of LLRs of each beat
                              (and lane) is given by a din_words packet for each beat
   DOUT_Words_Configuration   WORDS_PER_BLOCK: the number of output bytes of each
                              beat (and lane) is given by one dout_words packet for
                              each block
 With a words configuration that is not fixed, the corresponding words stream is an
 input of the decoder that the user drives (six AXI4-Stream channels here).
****************************************************************/
struct config : params_t {
    static const unsigned Standard                 = STD_5G;
    static const unsigned DIN_Words_Configuration  = WORDS_PER_TRANSACTION;
    static const unsigned DOUT_Words_Configuration = WORDS_PER_BLOCK;
};

void ldpc_top(hls::stream<ctrl_t>&               ctrl_s,
              hls::stream<din_bus<config>>&      din_s,
              hls::stream<din_words_t<config>>&  din_words_s,
              hls::stream<dout_bus<config>>&     dout_s,
              hls::stream<dout_words_t<config>>& dout_words_s,
              hls::stream<status_t>&             status_s);
