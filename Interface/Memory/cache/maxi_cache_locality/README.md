This example illustrates usage of cache when vitis HLS fails to burst

Files included in this package
==================================================
README  
dut.cpp  
dut_tb.cpp  
run.tcl

=================================================
vitis_hls -f run_hls.tcl

Note the following interface #pragma used:

#pragma HLS cache port=in lines=16 depth=512
