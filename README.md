<img src="./Images/logo2.png" alt="logo2" width="200"/>

# Introductory examples for Vitis HLS

Copyright 1986-2022 Xilinx, Inc. All Rights Reserved. 

Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## C/C++ synthesizable examples

Each example comes with C/C++ source code, a README, and script or config file.
The examples are organized in categories denoted by the directory names:
||||
|-|-|-|
| Appnotes| A DSP design, a legacy digital up converter appnote.|[Digital_Up_Converter](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/AppNotes/Digital_Up_Converter)|
| Array| Show how to partition memory arrays.|[array_partition_complete](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Array/array_partition_complete)|
| Interface| Common examples for interface protocols.|[using_axi_master](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Memory/using_axi_master)<br> [using_axi_lite](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Register/using_axi_lite)<br> [using_axi_stream_with_side_channel_data](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Streaming/using_axi_stream_with_side_channel_data)|
| Misc| Other examples such as the RTL blackbox flow and the LogiCore FFT from Vivado.|[fft](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Misc/fft)<br> [rtl_as_blackbox](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Misc/rtl_as_blackbox)|
| Modelling| The essentials for loops, arbitrary precision types and vectors.|[basic_loops_primer](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/basic_loops_primer)<br> [using_arbitrary_precision_arith](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_arbitrary_precision_arith)<br> [using_vectors](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_vectors)<br> [using_array_stencil_1d](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_array_stencil_1d)|
| Pipelining| Illustrating one of the most fundamental concept of HLS.|[hier_func](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Pipelining/Functions/hier_func)<br> [pipelined_loop](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Pipelining/Loops/pipelined_loop)|
| Task_Level_Parallelism| Dataflow and free running streams with hls::task.|[using_stream_of_blocks](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Control_driven/Channels/using_stream_of_blocks)<br> [input_bypass](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Control_driven/Patterns/input_bypass)<br> [unique_task_regions](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Data_driven/unique_task_regions)|

## Running the examples at command line

* Using Vitis Unified IDE:
  - ```vitis-run --mode hls --tcl run_hls.tcl```
* Using Vitis HLS (deprecated):
  - ```vitis_hls -f run_hls.tcl```

Note: By default only C simulation and C synthesis will run. Change the value of hls_exec in the Tcl script to run co-simulation and Vivado implementation.

