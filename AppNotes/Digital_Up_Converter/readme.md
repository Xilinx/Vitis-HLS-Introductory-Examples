
# Design Overview

This readme describes how to use the design files that come with XAPP1299.
These designs, which are described in detail in XAPP1299 are: C++ source, header files,
Tcl script file to create a HLS project.

# DESIGN FILE HIERARCHY

dds.h:        header file describing DDS class
duc.cpp:      DUC top level 
duc.h:        DUC header file
duc_test.cpp: testbench for DUC design
fir.h:        includes various types of FIR class
readme.txt:   this file
run.tcl:      TCL script to create a HLS project
run_2p5.tcl:  sets the clock period and call the run.tcl file

# Running the Example

Open a command prompt with vitis tools set up, go into the directory of a given design example and then run the command : 

```
$ vitis_hls -f run_hls.tcl
```
If you wish you can open the project with the VHLS GUI :

```
$ vitis_hls -p proj_*
```

# Performance

## Throughput
After running the design example, you can check the throughput in the GUI by opening the co-simulation report and verifying the II numbers for the top-level function range 3200 clock cycles which is expected for the DUC; you can also open the waveform if you enabled the dump trace option.

## Frequency

In terms of achievable frequency, the design example is setup in Vitis HLS for 2.5 ns (i.e. 400 MHz) targetting a kintex Ultrascale (xcku040-ffva1156-2-e); Using the export flow, we can check what frequency is achieved when run with the out-of-context implementation option. Different frequencies will be achieved if you change the target part and/or the clock period or, the design.

```
Implementation tool: Xilinx Vivado v.2023.1
Project:             proj_clk2.5
Solution:            solution1
Device target:       xcku040-ffva1156-2-e
Report date:         Mon May 01 11:41:10 PDT 2023

#=== Post-Implementation Resource usage ===
SLICE:            0
LUT:           5455
FF:            9809
DSP:             30
BRAM:            10
URAM:             0
LATCH:            0
SRL:            476
CLB:           1314

#=== Final timing ===
CP required:                     2.500
CP achieved post-synthesis:      2.755
CP achieved post-implementation: 2.420
Timing met

```



