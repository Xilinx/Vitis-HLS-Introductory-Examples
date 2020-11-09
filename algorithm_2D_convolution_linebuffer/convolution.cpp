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

void filter11x11_orig(int width, int height, const data_t *src, data_t *dst)
{
#pragma HLS INTERFACE m_axi port=src depth=32400 // TEST_IMG_SIZE
#pragma HLS INTERFACE m_axi port=dst depth=32400 // TEST_IMG_SIZE
#pragma HLS INTERFACE s_axilite port=width  bundle=hls_ctrl 
#pragma HLS INTERFACE s_axilite port=height bundle=hls_ctrl 
#pragma HLS INTERFACE s_axilite port=return bundle=hls_ctrl 

#pragma HLS INLINE
#pragma HLS DATAFLOW

  const data_t filt11_coeff[11] = {
    36, 111, 266, 498, 724, 821, 724, 498, 266, 111, 36
  };

  convolution_orig<data_t, 11>(width, height,
			       src, dst,
			       filt11_coeff, filt11_coeff);
}

void filter11x11_strm(int width, int height,
		      hls::stream<data_t> &src, hls::stream<data_t> &dst)
{
#pragma HLS INTERFACE axis port=&src 
#pragma HLS INTERFACE axis port=&dst 

#pragma HLS DATAFLOW
#pragma HLS INLINE // bring loops in sub-functions to this DATAFLOW region

  const data_t filt11_coeff[11] = {
    36, 111, 266, 498, 724, 821, 724, 498, 266, 111, 36
  };

  convolution_strm<data_t, 11>(width, height,
			       src, dst,
			       filt11_coeff, filt11_coeff);
}
