This example illustrates case of usefulness of cases in cases of Vitis HLS burst failures  

Files included in this package
==================================================
README  
dut.cpp  
dut_tb.cpp  
run.tcl

Running the design (edit x_hls.tcl to run synthesis)
=================================================
vitis_hls -f run_hls.tcl

Note the following interface #pragma used:

#pragma HLS cache port=in lines=8 depth=128
