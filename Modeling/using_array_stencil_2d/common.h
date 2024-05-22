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
#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_IMAGE_WIDTH     1000
#define MAX_IMAGE_HEIGHT    30

#define FILTER_V_SIZE		15
#define FILTER_H_SIZE		15

#ifndef MIN
#define MIN(a,b) ((a<b)?a:b)
#endif
#ifndef MAX
#define MAX(a,b) ((a<b)?b:a)
#endif
// SW version
void Filter2D(
		const char           coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
		float		         factor,
		short                bias,
		unsigned short       width,
		unsigned short       height,
		unsigned short       stride,
		const unsigned char src[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH],
		unsigned char       dst[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH]);

// HW version
extern "C" {

  void Filter2DKernel(
		const char           coeffs[256],
		const unsigned char src[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH],
		unsigned char       dst[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH]);

}


typedef unsigned char      		U8;
typedef unsigned short     		U16;
typedef unsigned int       		U32;

typedef signed char        		I8;
typedef signed short       		I16;
typedef signed int         		I32;
