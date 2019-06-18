/*******************************************************************************
Vendor: Xilinx 
Associated Filename: dds_chain.cpp
Purpose:Vivado HLS Design example for DDS  
Device: All 
Revision History: March 5, 2015-2019 - initial release
                                                
*******************************************************************************
#-  (c) Copyright 2011-2019 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include "dds_chain.h"
#include "hls_dds.h"

const double config1::PINC[16] = { 8192 };
const double config1::POFF[16] = { 0 };
const double config1::DDS_Clock_Rate = 25.0;

template<typename data_t, int LENGTH>
void dummy_process(data_t in[LENGTH], data_t out[LENGTH]) {
    for (unsigned i = 0; i < LENGTH; ++i)
        out[i] = in[i];
}

template<typename data_t, int LENGTH>
void dummy_process2(hls::ip_dds::out_data_sin_cos<config1> in[LENGTH],
        data_t sin[LENGTH], data_t cosin[LENGTH]) {
    for (unsigned i = 0; i < LENGTH; ++i) {
        sin[i] = in[i].get_sin();
        cosin[i] = in[i].get_cos();
    }
}

// DUT
void dds_top(data_data_t sin[DDS_LENGTH], data_data_t cos[DDS_LENGTH],
        data_phase_t phase[DDS_LENGTH]) {
#pragma HLS INTERFACE axis depth=128 port=sin
#pragma HLS INTERFACE axis depth=128 port=cos
#pragma HLS INTERFACE axis depth=128 port=phase
#pragma HLS stream variable=sin
#pragma HLS stream variable=cos
#pragma HLS stream variable=phase
#pragma HLS dataflow
    hls::ip_dds::out_data_sin_cos<config1> data_channel[DDS_LENGTH];
    data_phase_t phase_channel[DDS_LENGTH];

    // Create DDS instance
    static hls::DDS<config1> dds1;

    //==================================================
    // Dataflow process
    dds1.run(data_channel, phase_channel);
    dummy_process<data_phase_t, DDS_LENGTH>(phase_channel, phase);
    dummy_process2<data_data_t, DDS_LENGTH>(data_channel, sin, cos);
    //==================================================
}

