/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include "fir_top.h"

struct config1 : hls::ip_fir::params_t {
    static const double coeff_vec[total_num_coeff];
    static const unsigned input_length = INPUT_LENGTH;
    static const unsigned output_length = OUTPUT_LENGTH;
    static const unsigned num_coeffs = COEFF_NUM;
    static const unsigned coeff_sets = COEFF_SETS;
    static constexpr double sample_period = 0.5;
    static const unsigned filter_type = hls::ip_fir::decimation;
    static const unsigned rate_change = hls::ip_fir::integer;
    static const unsigned decim_rate = 2;
};
const double config1::coeff_vec[total_num_coeff] = 
    {6,0,-4,-3,5,6,-6,-13,7,44,64,44,7,-13,-6,6,5,-3,-4,0,6};

// DUT
void fir_top(s_data_t in[INPUT_LENGTH],
             m_data_t out[OUTPUT_LENGTH],
             config_t* config)
{
//#pragma HLS interface ap_fifo depth=INPUT_LENGTH port=in
//#pragma HLS interface ap_fifo depth=OUTPUT_LENGTH port=out
//#pragma HLS stream variable=in
//#pragma HLS stream variable=out
#pragma HLS dataflow

    // Create struct for config
    static hls::FIR<config1> fir1;
    
    fir1.run(in, out, config);
}