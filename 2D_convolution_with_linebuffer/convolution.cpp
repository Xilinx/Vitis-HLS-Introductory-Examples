/*******************************************************************************
Vendor: Xilinx 
Associated Filename: convolution.cpp
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
#include "convolution.h"

template<typename T, int K>
static void convolution_orig(
        int width, int height,
        const T *src, T *dst,
        const T *hcoeff, const T *vcoeff)
{
    // Convolution kernel size
    const int conv_size = K;
    // Half the convolution window - rounded down - i.e. the border width
    const int border_width = int(conv_size / 2);
#ifndef __SYNTHESIS__
    T * const local = new T[MAX_IMG_ROWS*MAX_IMG_COLS];
#else // Static storage allocation for HLS, dynamic otherwise
    T local[MAX_IMG_ROWS*MAX_IMG_COLS];
#endif

    // Clear local frame buffer
    Clear_Local:for(int i = 0; i < height * width; i++){
        local[i]=0;
    }
    // Horizontal convolution pass - makes O(K*K) reads from input image
    // per output pixel
    HconvH:for(int col = 0; col < height; col++){
        HconvW:for(int row = border_width; row < width - border_width; row++){
            Hconv:int pixel = col * width + row;
            for(int i = - border_width; i <= border_width; i++){
                local[pixel] += src[pixel + i] * hcoeff[i + border_width];
            }
        }
    }
    // Clear dst storage
    Clear_Dst:for(int i = 0; i < height * width; i++){
        dst[i]=0;
    }
    // Vertical convolution pass - makes O(K*K) reads from frame buffer -
    // resulting in only interior, i.e.
    // (border_width < col < height - border_width && border_width < row < width - border_width), pixels being valid
    VconvH:for(int col = border_width; col < height - border_width; col++){
        VconvW:for(int row = 0; row < width; row++){
            int pixel = col * width + row;
            Vconv:for(int i = - border_width; i <= border_width; i++){
                int offset = i * width;
                dst[pixel] += local[pixel + offset] * vcoeff[i + border_width];
            }
        }
    }
    // Populate borders by replicating adjacent valid pixels - uses a separate
    // set of loop nest for each vertical border region - top border; left/right
    // of valid vertical range; bottom. This is problematic for performance...
    int border_width_offset = border_width * width;
    int border_height_offset = (height - border_width - 1) * width;
    Top_Border:for(int col = 0; col < border_width; col++){
        int offset = col * width;
        Top_Left:for(int row = 0; row < border_width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_width_offset + border_width];
        }
        Top_Row:for(int row = border_width; row < width - border_width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_width_offset + row];
        }
        Top_Right:for(int row = width - border_width; row < width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_width_offset + width - border_width - 1];
        }
    }
    Side_Border:for(int col = border_width; col < height - border_width; col++){
        int offset = col * width;
        Left_Col:for(int row = 0; row < border_width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[offset + border_width];
        }
        Right_Col:for(int row = width - border_width; row < width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[offset + width - border_width - 1];
        }
    }
    Bottom_Border:for(int col = height - border_width; col < height; col++){
        int offset = col * width;
        Bottom_Left:for(int row = 0; row < border_width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_height_offset + border_width];
        }
        Bottom_Row:for(int row = border_width; row < width - border_width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_height_offset + row];
        }
        Bottom_Right:for(int row = width - border_width; row < width; row++){
            int pixel = offset + row;
            dst[pixel] = dst[border_height_offset + width - border_width - 1];
        }
    }
}

template<typename T, int K>
static void convolution_strm(int width, int height,
        hls::stream<T> &src, hls::stream<T> &dst,
        const T *hcoeff, const T *vcoeff)
{
    const int border_width = int(K / 2);
    // Horizontal pixel window (cache)
    T hwin[K];
    hls::stream<T> hconv("hconv");
    // Vertical pixel window (cache)
//    T vwin[K];
    // Line-buffers allowing full pixel reuse in vertical pass
    static T linebuf[K - 1][MAX_IMG_COLS];
    hls::stream<T> vconv("vconv");
    const int vconv_xlim = width - (K - 1);
    // Line-buffer for border pixel replication
    T borderbuf[MAX_IMG_COLS - (K - 1)];
#pragma HLS ARRAY_PARTITION variable=linebuf dim=1 complete
#pragma HLS INLINE // Into a DATAFLOW region
    // These assertions let HLS know the upper bounds of loops
    assert(height < MAX_IMG_ROWS);
    assert(width < MAX_IMG_COLS);
    assert(vconv_xlim < MAX_IMG_COLS - (K - 1));
    // Horizontal convolution - consumes each pixel in source image
    // exactly once, reusing values cached in hwin[], producing a stream
    // of pixels required for the following vertical convolution
    HConvH:for(int col = 0; col < height; col++) {
        HConvW:for(int row = 0; row < width; row++) {
#pragma HLS PIPELINE
            T in_val = src.read();
            // Reset pixel value on-the-fly - eliminates an O(height*width) loop
            T out_val = 0;
            HConv:for(int i = 0; i < K; i++) {
                hwin[i] = i < K - 1 ? hwin[i + 1] : in_val;
                out_val += hwin[i] * hcoeff[i];
            }
            if (row >= K - 1)
                hconv << out_val;
        }
    }
    // Vertical convolution - consumes stream generated by the horizontal
    // pass; generates a stream of only the pixels in the valid interior
    // region, i.e. (height - (K - 1)) * (width - (K - 1)) values
    VConvH:for(int col = 0; col < height; col++) {
        VConvW:for(int row = 0; row < vconv_xlim; row++) {
#pragma HLS DEPENDENCE variable=linebuf inter false
#pragma HLS PIPELINE
            T in_val = hconv.read();
            // Reset pixel value on-the-fly - eliminates an O(height*width) loop
            T out_val = 0;
            VConv:for(int i = 0; i < K; i++) {
                T vwin_val = i < K - 1 ? linebuf[i][row] : in_val;
                out_val += vwin_val * vcoeff[i];
                if (i > 0)
                    linebuf[i - 1][row] = vwin_val;
            }
            if (col >= K - 1)
                vconv << out_val;
        }
    }
    //Handle border by replicating the exact same pixels as orig, but in
    // a single loop taking the minimum (height*width) number of cycles
    Border:for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            T pix_in, l_edge_pix, r_edge_pix, pix_out;
#pragma HLS PIPELINE
            if (i == 0 || (i > border_width && i < height - border_width)) {
                // read a pixel out of the input stream and cache it for
                // immediate use and later replication purposes
                if (j < width - (K - 1)) {
                    pix_in = vconv.read();
                    borderbuf[j] = pix_in;
                }
                if (j == 0) {
                    l_edge_pix = pix_in;
                }
                if (j == width - K) {
                    r_edge_pix = pix_in;
                }
            }
            // Select output value from the appropriate cache resource
            if (j <= border_width) {
                pix_out = l_edge_pix;
            } else if (j >= width - border_width - 1) {
                pix_out = r_edge_pix;
            } else {
                pix_out = borderbuf[j - border_width];
            }
            dst << pix_out;
        }
    }
}

void filter11x11_orig(
        int width, int height,
        const data_t *src, data_t *dst)
{
#pragma HLS INTERFACE port=src ap_bus depth=32400 // TEST_IMG_SIZE
#pragma HLS RESOURCE variable=src core=AXI4M
#pragma HLS INTERFACE port=dst ap_bus depth=32400 // TEST_IMG_SIZE
#pragma HLS RESOURCE variable=dst core=AXI4M
#pragma HLS RESOURCE variable=return core=AXI4LiteS metadata="-bus_bundle hls_ctrl"
#pragma HLS RESOURCE variable=width      core=AXI4LiteS metadata="-bus_bundle hls_ctrl"
#pragma HLS RESOURCE variable=height      core=AXI4LiteS metadata="-bus_bundle hls_ctrl"

#pragma HLS INLINE region
#pragma HLS DATAFLOW

    const data_t filt11_coeff[11] = {
            36, 111, 266, 498, 724, 821, 724, 498, 266, 111, 36
    };

    convolution_orig<data_t, 11>(width, height,
            src, dst,
            filt11_coeff, filt11_coeff);
}

void filter11x11_strm(
        int width, int height,
        hls::stream<data_t> &src, hls::stream<data_t> &dst)
{
#pragma HLS INTERFACE axis port=&src 
#pragma HLS INTERFACE axis port=&dst 

#pragma HLS DATAFLOW
#pragma HLS INLINE region // bring loops in sub-functions to this DATAFLOW region

    const data_t filt11_coeff[11] = {
            36, 111, 266, 498, 724, 821, 724, 498, 266, 111, 36
    };

    convolution_strm<data_t, 11>(width, height,
            src, dst,
            filt11_coeff, filt11_coeff);
}

