#include "example.h"

void krnl_transfer(int* in,int* out, const int size ) {
  #pragma HLS INTERFACE m_axi port=in depth=512
  #pragma HLS INTERFACE m_axi port=out depth=5120
        int buf[8192];
		for (int i = 0; i < size; i++) {
		#pragma HLS PIPELINE II=1
		buf[i] = in[i];
		}
	    
        for (int i = 0; i < NT; i++) {
	        for (int j = 0; j < size; j++) {
#pragma HLS PIPELINE II=1
	            out[j+size*i] = buf[j];
	        }
	    }


	}

