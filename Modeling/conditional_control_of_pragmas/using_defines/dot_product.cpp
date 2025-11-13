#include "dot_product.h"
#define OPT 3

int dot_product(int A[SIZE], int B[SIZE]) {
#pragma HLS INTERFACE mode=ap_memory port=A
#pragma HLS INTERFACE mode=ap_memory port=B
#pragma HLS INTERFACE mode=s_axilite port=return

    int result = 0;

   
    DP_LOOP: for (int i = 0; i < SIZE; i++) {    
        
    #pragma HLS if (OPT==1) PIPELINE II=1       // Version 1
    #pragma HLS if (OPT==2) UNROLL factor=16    // Version 2
    #pragma HLS if (OPT==3) UNROLL factor=4     // Version 3
    #pragma HLS if (OPT==3) PIPELINE II=2       // Version 3 (combine with above)
                
        result += A[i] * B[i];
    }

    return result;
}

