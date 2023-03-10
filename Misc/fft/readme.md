Description
===========

This C++ design is illustrating the use of the Xilinx FFT IP-XACT IP in Vitis HLS. This example is a single 1024 point forward FFT.

The architecture is using dataflow with 3 processes : 
* a datamover to read the input data,
* a process to call the FFT itself,
* a datamover to write-out the output data. 


```
     +--(fft_top)------------------------------------------------------------+
     |                                                                       |
in --+--> [inputdatamover]--(xn)-->[myfftwrapper]--(xk)-->[outputdatamover]--+--> out
     |                                                                       |
     +-----------------------------------------------------------------------+
```

<<<<<<< HEAD
The FFT IP instantiation in VHLS supports 2 access modes: via arrays or via hls::stream<>.
The example design top level function was also created with the same 2 possibilities: arrays or via hls::stream<>. 

Hence, there are 4 variations of the design which should help you adapt this example to your design. They are referenced in the table below.
=======
The FFT IP instantiation in VHLS supports 2 access modes: via arrays or via hls::stream<>; those are the types you can define the variables "xn" and "xk" in the above diagram.
The example design top level function argument type for the input "in" and output "out" was also created with those 2 possibilities so there are also 2 possible implementations.

Hence there are 4 variations of the design which should help you adapt this example to your design. They are referenced in the table below. If you integrate the FFT into a dataflow region you should not need the datamovers.
>>>>>>> 56fbe12df3a731dbcb13de5e4e3a94446f6f9c68

| Design name             | Top level interfaces | Internal data types |
|---------------------------------------------|--------|--------|
| fft_single_interface_array_internal_array   | array  | array  |
| fft_single_interface_array_internal_stream  | array  | stream |
| fft_single_interface_stream_internal_array  | stream | array  |
| fft_single_interface_stream_internal_stream | stream | stream |

At the time this github example was written, there were small differences in the VHLS tool versions and their support for the FFT IP so there is code specific to 2022.2 and 2023.1 versions. They have been created so that each can get the optimal performance.

Design Files
============
Each design variations has the following files:

data        : directory with input and expected data used by the testbench

fft_tb.cpp  : C testbench; calls 20 times the top function

fft_top.cpp : top C function fft_top

ffp_top.h   : header file for the example 

run_hls.tcl : script to run synthesis, simulation and export IP

README      : a small readme file which refers to this readme.md

Running the Example
===================
Open a command prompt with vitis tools set up, go into the directory of a given design example and then run the command : 
```
$ vitis_hls -f run_hls.tcl
```
If you wish you can open the project with the VHLS GUI :
```
$ vitis_hls -p proj_fft_single*
```
We also provide a bash script that can be used to run the 4 variations or a specific version with the syntax : 
```
$ ./run_all.bash 2022.2
```
or
```
$ ./run_all.bash 2022.2/fft_single_interface_array_internal_array/
```

Performance
===========

## Throughput
After running the design example, you can check the throughput in the GUI by opening the co-simulation report and verifying the II numbers for the top-level function fft_top which will be in the range 1024 - 1027 clock cycles which is expected for the 1024 point FFT; you can also open the waveform if you enabled the dump trace option.
On the command line you can grep the output of the latency report, for example 
```
$ grep THROUGHPUT 2022.2/fft_single_interface_array_internal_stream
$MAX_THROUGHPUT = "1025"
$MIN_THROUGHPUT = "1024"
$AVER_THROUGHPUT = "1024"
```

## Frequency
In terms of achievable frequency, the design example is setup in Vitis HLS for 3.3 ns (i.e. 300 MHz) targetting a U200 Alveo board (part xcvu9p-flga2104-2-i) and can achieve this if the export flow is run with the out-of-context implementation option. 

The following results were achieved with the tools version 2022.2 and the fft_single_interface_stream_internal_stream example (also in `<proj>/solution1/impl/report/verilog/fft_top_export.rpt`) and show that the critical path of the out-of-context place and route achieved just below 2.5 ns. Different frequency will be achieved if you change the target part or the clock period or the design.

```
Implementation tool: Xilinx Vivado v.2022.2
Project:             proj_fft_single_interface_stream_internal_stream_vitis_hls_2022.2
Solution:            solution1
Device target:       xcvu9p-flga2104-2-i

#=== Post-Implementation Resource usage ===
SLICE:            0
LUT:           3325
FF:            4723
DSP:             12
BRAM:             7
URAM:             0
LATCH:            0
SRL:            986
CLB:            809

#=== Final timing ===
CP required:                     3.300
CP achieved post-synthesis:      1.756
CP achieved post-implementation: 2.477
Timing met
```

Points to Note 
===============
You will see:
- in the tcl script setup: -start_fifo_depth 4 is used,
- a wrapper around the fft call is sometimes used to improve the performance.

We are aiming the remove the above 2 restrictions.

Furthermore, for co-simulation we are using the option -svuvm_nostall to remove some cosimulation artifacts which may wrongly show gaps in the input data between 2 runs, and you will see that the cosimulation succeeds with typical message : `INFO: [COSIM 212-1000] *** C/RTL co-simulation finished: PASS ***`.

<<<<<<< HEAD
However due to the use of the -svuvm_nostall option, the flow will issue an error message which you can discard : `ERROR: [HLS 200-644] The 'cosim_design -svuvm_nostall' hidden command is not supported.` ... `command 'cosim_design' returned error code while executing "cosim_design -rtl verilog -svuvm_nostall" invoked from within` ...
=======
We are aiming the remove the above 2 restrictions
- for co-simulation we are using the option -svuvm_nostall to remove some cosimulation artifacts which may wrongly show gaps in the input data between 2 runs.
>>>>>>> 56fbe12df3a731dbcb13de5e4e3a94446f6f9c68
