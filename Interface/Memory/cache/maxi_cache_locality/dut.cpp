extern "C" {
void dut(const double* in, // Read-Only Vector 1
         double* out,      // Output Result
         int size          // Size in integer
) {

#pragma HLS INTERFACE mode = m_axi bundle = aximm2 depth =                     \
    1025 max_read_burst_length = 128 num_read_outstanding = 256 port = in
#pragma HLS INTERFACE m_axi port = out bundle = aximm depth = 1024

#pragma HLS cache port = in lines = 1 depth = 128

    for (int i = 0; i < size; i++) {
        out[i] = in[i] + in[i + 1];
    }
}
}
