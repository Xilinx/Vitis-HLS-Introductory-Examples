This example illustrates using Load-Compute-Store using a vector datatype.

Note the following interface #prgama used:

#pragma HLS pipeline
#pragma HLS INTERFACE m_axi port=lhs offset=slave bundle=gmem0 depth=32
#pragma HLS INTERFACE m_axi port=rhs offset=slave bundle=gmem1 depth=32
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=gmem0 depth=32
#pragma HLS DATAFLOW

