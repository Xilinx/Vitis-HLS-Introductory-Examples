<img src="./Images/logo2.png" alt="logo2" width="200"/>

# Introductory examples for Vitis HLS

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at [Apache Licence, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## C/C++ synthesizable examples

Each example comes with C/C++ source code, testbench, a README, and Tcl/Python scripts and/or config file.
The examples are organized in categories denoted by the directory names:
|Category|Description|Key Examples|
|-|-|-|
| DSP| Shows DSP Intrinsic Library and Vivado LogiCore FFT usage.|[DSP_Intrinsic_Library](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/DSP/DSP_Intrinsic_Library)<br>[fft](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/DSP/fft)|
| Array| Show how to partition memory arrays.|[array_partition_complete](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Array/array_partition_complete)<br>[array_partition_block_cyclic](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Array/array_partition_block_cyclic)|
| Interface| Common examples for interface protocols.|[using_axi_master](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Memory/using_axi_master)<br> [using_axi_lite_with_user_defined_offset](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Register/using_axi_lite_with_user_defined_offset)<br> [using_axi_stream_with_side_channel_data](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Interface/Streaming/using_axi_stream_with_side_channel_data)|
| Modeling| The essentials for loops, arbitrary precision types and vectors. Condtional control of HLS Pragmas.|[variable_bound_loops](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/variable_bound_loops)<br> [using_arbitrary_precision_arith](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_arbitrary_precision_arith)<br> [using_vectors](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_vectors)<br> [using_array_stencil_1d](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/using_array_stencil_1d)<br> [conditional_control_of_pragmas](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Modeling/conditional_control_of_pragmas)|
| Pipelining| Illustrating one of the most fundamental concept of HLS.|[hier_func](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Pipelining/Functions/hier_func)<br> [pipelined_loop](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Pipelining/Loops/pipelined_loop)|
| Task_Level_Parallelism| Dataflow and free running streams with ```hls::task```. Autorestart support in testbench.|[using_stream_of_blocks](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Control_driven/Channels/using_stream_of_blocks)<br> [autorestart](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Control_driven/Channels/autorestart)<br> [unique_task_regions](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Data_driven/unique_task_regions)<br>[using_directio_hs_in_tasks](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Task_level_Parallelism/Data_driven/using_directio_hs_in_tasks)|
| Misc| Other examples such as the RTL blackbox flow.|[rtl_as_blackbox](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Misc/rtl_as_blackbox)|
| Migration| Examples covering scripted and command-line migration flows to Vitis Unified IDE.|[tcl_scripts](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Migration/tcl_scripts)<br> [python_scripts](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Migration/python_scripts)<br> [vitis_unified_cli](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/tree/master/Migration/vitis_unified_cli)|

## Running the example scripts using Vitis Unified IDE

|Script Type|Command|Notes|
|-|-|-|
|Tcl|```vitis-run --mode hls --tcl run_hls.tcl```|Open the directory containing `run_hls.tcl` as workspace after running the Tcl script to open in Vitis Unified IDE|
|Python|```vitis -s run.py```|Open the created directory `w` as workspace after running the python script to open in Vitis Unified IDE|

By default C Simulation, C Synthesis and Co-Simulation are run with both Tcl and Python scripts. Modify respective script to run Implementation and Packaging.

## Documentation

[Vitis High-Level Synthesis User Guide (UG1399)](https://docs.amd.com/r/en-US/ug1399-vitis-hls)

## Additional Resources

[Vitis High-Level Synthesis - Useful Resources](https://adaptivesupport.amd.com/s/question/0D5KZ00000h7Q900AE/vitis-hls-useful-resources?language=en_US)

[Parallel Programming for FPGAs](https://github.com/Xilinx/pp4fpgas)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
