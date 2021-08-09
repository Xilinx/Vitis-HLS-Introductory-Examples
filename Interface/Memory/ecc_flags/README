This example illustrates how to infer an error code correction ultra-RAM using BIND_STORAGE. 

Note the following interface #prgama used:

#pragma HLS ARRAY_RESHAPE variable=res1 dim=1 factor=2 block
#pragma HLS BIND_STORAGE variable=res1 type=ram_s2p impl=uram_ecc
