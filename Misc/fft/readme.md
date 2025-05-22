Description
===========

This C++ design is illustrating the use of the AMD/Xilinx FFT IP-XACT IP in Vitis HLS. This example is a single 1024 point forward FFT.

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

The FFT C++ instantiation supports 2 access modes: via arrays or via hls::stream<>; those are the types you can define the variables "xn" and "xk" in the above diagram. In the 2 example designs that we provide, the top level function arguments (the input "in" and output "out") are created with the same types as the internal type. 

We recommend using the stream version.

The 2 variations of the design are referenced in the table below. If you integrate the FFT into a dataflow region you may not need the datamovers.


|   Design name    | Top level interfaces | Internal data types |
|:----------------:|:--------------------:|:-------------------:|
| interface_array  |         array        |         array       |
| interface_stream |        stream        |        stream       |

The design example has been tested with version 2023.1 of Vitis HLS.

Design Files
============
Each design variations has the following files:

data        : directory with input and expected data used by the testbench
fft_tb.cpp  : C testbench; calls 20 times the top function
fft_top.cpp : top C function fft_top
ffp_top.h   : header file for the example 
run_hls.tcl : script to run synthesis, simulation and export IP
run.py	    : script to run csimulation, csynthesis and cosimulation using vitis
README      : a small readme file which refers to this readme.md

Running the Example
===================
Open a command prompt with vitis tools set up, go into the directory of a given design example and then run the command : 
```
$ vitis_hls -f run_hls.tcl
```
If you wish you can open the project with the VHLS GUI :
```
$ vitis_hls -p proj_*
```
Run python script with vitis
```
$ vitis -s run.py
```
To open the HLS component in Vitis Unified IDE after running the python script:
```
$ vitis -w w
```


Performance
===========

## Throughput
After running the design example, you can check the throughput in the GUI by opening the co-simulation report and verifying the II numbers for the top-level function fft_top which will be in the range 1025 - 1027 clock cycles which is expected for the 1024 point FFT; you can also open the timeline trace or the Vivado waveform if you enabled the dump trace option.
On the command line you can grep the output of the latency report, for example 
```
$ for d in interface_* ; do echo $d ; grep THROUGHPUT $d/proj*/solution1/sim/report/verilog/lat.rpt ; done

interface_array
$MAX_THROUGHPUT = "1027"
$MIN_THROUGHPUT = "1026"
$AVER_THROUGHPUT = "1026"
interface_stream
$MAX_THROUGHPUT = "1025"
$MIN_THROUGHPUT = "1024"
$AVER_THROUGHPUT = "1024"
```

## Frequency
This design example has a 2.5 ns period constraint (i.e. 400 MHz) targetting a xcvu9p-flga2104-2-i; The export flow allows us to run the IP in Vivado. 
Your result may vary depending on the target devive you choose, the constraint you pick or the context in which the IP block is integrated.

```
================================================================
== Vivado Place & Route Results
================================================================
+ General Information:
    * Version:         2025.1 (Build 6135595 on May 19 2025)
    * Project:         component_interface_array
    * Solution:        hls (Vivado IP Flow Target)
    * Product family:  virtexuplus
    * Target device:   xcvu9p-flga2104-2-i

================================================================
== Place & Route Resource Summary
================================================================
LUT:              3317
FF:               4703
DSP:              12
BRAM:             7
URAM:             0
SRL:              979

================================================================
== Place & Route Timing Summary
================================================================
* Timing was met
+----------------+-------------+
| Timing         | Period (ns) |
+----------------+-------------+
| Target         | 2.500       |
| Post-Synthesis | 1.569       |
| Post-Route     | 2.324       |
+----------------+-------------+
```

Points to Note 
===============
You will see:
- in the tcl script setup: -start_fifo_depth 4 is used,
- a wrapper around the fft call is sometimes used to improve the performance.

We are aiming the remove the above 2 restrictions.
