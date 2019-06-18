/*******************************************************************************
Vendor: Xilinx 
Associated Filename: convolution_test.cpp
Purpose: Vivado HLS Design Example 
Device: All 
# Revision History: Oct 1, 2014-2019 - initial release
                                                
*******************************************************************************
© Copyright 2008 – 2012 Xilinx, Inc. All rights reserved. 

This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the fibon_fimum extent permitted by applicable law: 
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
#include <iostream>
#include <cstdlib>

#include "convolution.h"

using namespace std;

int main(void)
{
    data_t * const src_img = new data_t[TEST_IMG_ROWS*TEST_IMG_COLS];
    data_t * const ref_img = new data_t[TEST_IMG_ROWS*TEST_IMG_COLS];
    hls::stream<data_t> src_img_strm("src_img_strm");
    hls::stream<data_t> dut_img_strm("dut_img_strm");
    const int chkr_size = 5;
    const data_t max_pix_val = 255;
    const data_t min_pix_val = 0;
    int err_cnt = 0;
    int ret_val = 20;
    // Generate source image with a fixed test pattern - checker-board
    for (int i = 0; i < TEST_IMG_ROWS; i++) {
        data_t chkr_pair_val[2];
        if ((i / chkr_size) % 2 == 0) {
            chkr_pair_val[0] = max_pix_val; chkr_pair_val[1] = min_pix_val;
        } else {
            chkr_pair_val[0] = min_pix_val; chkr_pair_val[1] = max_pix_val;
        }
        for (int j = 0; j < TEST_IMG_COLS; j++) {
            data_t pix_val = chkr_pair_val[(j / chkr_size) % 2];
            src_img[i * TEST_IMG_COLS + j] = pix_val;
            src_img_strm << pix_val;
        }
    }
    // Generate reference convolution image
    filter11x11_orig(TEST_IMG_COLS, TEST_IMG_ROWS, src_img, ref_img);
    // Generate DUT convolution image
    filter11x11_strm(TEST_IMG_COLS, TEST_IMG_ROWS, src_img_strm, dut_img_strm);
    // Check DUT vs reference result
    for (int i = 0; i < TEST_IMG_ROWS; i++) {
        for (int j = 0; j < TEST_IMG_COLS; j++) {
            data_t dut_val = dut_img_strm.read();
            data_t ref_val = ref_img[i * TEST_IMG_COLS + j];
            if (dut_val != ref_val) {
                err_cnt++;
#if 0
                cout << "!!! ERROR: Mismatch detected at coord(" << i;
                cout << ", " << j << " ) !!!";
                cout << endl;
#endif
            }
        }
    }
    cout << endl;

    if (err_cnt == 0) {
        cout << "*** TEST PASSED ***" << endl;
        ret_val = 0;
    } else {
        cout << "!!! TEST FAILED - " << err_cnt << " mismatches detected !!!";
        cout << endl;
        ret_val = -1;
    }

    delete [] src_img;
    delete [] ref_img;

    return ret_val;
}

