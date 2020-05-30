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
    // Generate the source image with a fixed test pattern - checker-board
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

