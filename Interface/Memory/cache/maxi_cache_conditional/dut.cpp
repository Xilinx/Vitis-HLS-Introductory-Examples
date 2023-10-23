extern "C" {
void dut(const unsigned int* in, // Read-Only Vector 1
         unsigned int* out,      // Output Result
         int size                // Size in integer
) {

#pragma HLS INTERFACE m_axi port = in bundle = aximm depth = 1024
#pragma HLS INTERFACE m_axi port = out bundle = aximm depth = 1024

// new cache pragma
#pragma HLS cache port = in lines = 8 depth = 128

    for (int i = 0; i < size; i++) {
#pragma HLS pipeline II = 1
        int t;
        if (i != 512)
            t = in[i] + 1;
        else
            t = in[i];
        out[i] = t;
    }
}
}
