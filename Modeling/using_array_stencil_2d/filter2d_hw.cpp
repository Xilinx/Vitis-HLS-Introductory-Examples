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

#include "common.h"
#include "hls_stream.h"

extern "C" {
void Filter2DKernel(
		const char           coeff[256],
		const unsigned char src[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH],
		unsigned char       dst[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH])
{
#pragma HLS INTERFACE mode=m_axi port=src bundle=gmem1 depth=MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT
#pragma HLS INTERFACE mode=m_axi port=coeff bundle=gmem1 
#pragma HLS INTERFACE mode=m_axi port=dst bundle=gmem1 depth=MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT
#pragma HLS INTERFACE s_axilite port = src bundle = control
#pragma HLS INTERFACE s_axilite port = dst bundle = control
#pragma HLS INTERFACE s_axilite port = coeff bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

    char coeffs[FILTER_V_SIZE][FILTER_H_SIZE];
    float factor = 1.0/(FILTER_V_SIZE*FILTER_H_SIZE);
    short bias   = 0;
// Load the coefficients into local storage
    load_coefs: for (int i=0; i<FILTER_V_SIZE; i++) {
        for (int j=0; j<FILTER_H_SIZE; j++) {
#pragma HLS PIPELINE II=1
            coeffs[i][j] = coeff[i];
        }
    }


    for(int y=0; y<30; ++y)
    {
        for(int x=0; x<1000; ++x)
        {
#pragma HLS pipeline II=1
#pragma HLS array_stencil variable=src

        	// Apply 2D filter to the pixel window
			int sum = 0;
			for(int row=0; row<FILTER_V_SIZE; row++)
			{

				for(int col=0; col<FILTER_H_SIZE; col++)
				{
					unsigned char pixel;
					int xoffset = (x+col-(FILTER_H_SIZE/2));
					int yoffset = (y+row-(FILTER_V_SIZE/2));
					// Deal with boundary conditions : clamp pixels to 0 when outside of image 
					if ( (xoffset<0) || (xoffset>=1000) || (yoffset<0) || (yoffset>=30) ) {
						pixel = 0;
					} else {
						pixel = src[yoffset][xoffset];
					}
					sum += pixel*coeffs[row][col];
				}
			}
			
        	// Normalize and saturate result
			unsigned char outpix = MIN(MAX((int(factor * sum)+bias), 0), 255);

			// Write output
           	dst[y][x] = outpix;
        }
    }
}


}
