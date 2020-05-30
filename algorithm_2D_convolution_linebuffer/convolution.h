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

#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#include <assert.h>
#include <stdint.h>
#include <hls_stream.h>

#define MAX_IMG_ROWS 1080
#define MAX_IMG_COLS 1920

#define TEST_IMG_ROWS 135
#define TEST_IMG_COLS 240
#define TEST_IMG_SIZE (TEST_IMG_ROWS * TEST_IMG_COLS)

typedef uint32_t data_t;

// External function prototypes
void filter11x11_orig(
        int w, int h,
        const data_t *src_image, data_t *dst_image);

void filter11x11_strm(
        int w, int h,
        hls::stream<data_t> &src_image, hls::stream<data_t> &dst_image);

#endif // CONVOLUTION_H_ not defined

