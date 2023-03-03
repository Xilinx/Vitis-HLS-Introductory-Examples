Description
===========

This C++ design is illustrating the use of the Xilinx FFT IP-XACT IP in Vitis HLS. This example is a single 1024 point forward FFT.

The architecture is using dataflow with 3 processes : 
* a datamover to read the input,
* a process to call the FFT itself,
* a datamover to write out the output. 


```
     +--(fft_top)-----------------------------------------------------------+
     |                                                                      |
in --+--> [inputdatamover]--(xn)-->[myfftwrapper]--(xk)-->[inputdatamover]--+--> out
     |                                                                      |
     +----------------------------------------------------------------------+
```

The FFT IP instantiation in VHLS supports 2 access modes: via arrays or via hls::stream<>.
The example design top level function was also created with the 2 possibilities so there are also 2 possible implementations. 

Hence there are 4 variations of the design which should help you adapt this example to your design. They are referenced in the table below.

| Design name             | Top level interfaces | Internal data types |
|---------------------------------------------|--------|--------|
| fft_single_interface_array_internal_array   | array  | array  |
| fft_single_interface_array_internal_stream  | array  | stream |
| fft_single_interface_stream_internal_array  | stream | array  |
| fft_single_interface_stream_internal_stream | stream | stream |

At the time this github example was written, there were small differences in the VHLS tool versions and their support for the FFT IP so there is code specific to 2022.2 and 2023.1 versions. They have been created so that each can get the optimal performance.

Files
=======
Each design has the following files:

data        : directory with input and expected data used by the testbench

fft_tb.cpp  : C testbench; calls 20 times the top function

fft_top.cpp : top C function fft_top

ffp_top.h   : header file for the example 

run_hls.tcl : script to run synthesis, simulation and export IP

README      : so small readme file which refers to this readme.md

Running the Example
======================
Open a command prompt with vitis tools set up, go into the directory of a given design example and then run the command : 
```bash
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

Points to Note 
===============
- in the tcl script setup -start_fifo_depth 4 is used 
- a wrapper around the fft call is sometimes used to improve the performance

We are aiming the remove the above 2 restrictiong
- for co-simulation we are using the option -svuvm_nostall to remove some cosimulation artifacts which may wrongly show gaps in the input data between 2 runs.
