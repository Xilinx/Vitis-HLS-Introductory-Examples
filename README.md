<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AMD Vitis™ HLS Introductory Examples</h1>
    </td>
 </tr>
</table>

## C/C++ synthesizable examples

Each example comes with C/C++ source code, testbench, a README, and Tcl/Python scripts and/or config file.
The examples are organized in categories denoted by the directory names:
|Category|Description|Key Examples|
|-|-|-|
| DSP| Shows DSP Intrinsic Library and Vivado LogiCore FFT, LDPC and FIR usage in Vitis HLS.|[DSP_Intrinsic_Library](DSP/DSP_Intrinsic_Library)<br>[fft](DSP/fft)<br>[ldpc](DSP/ldpc)<br>[fir/decimator](DSP/fir/decimator)|
| Array| Show how to partition memory arrays.|[array_partition_complete](Array/array_partition_complete)<br>[array_partition_block_cyclic](Array/array_partition_block_cyclic)<br>[array_partition_lib_simple](Array/array_partition_lib_simple)|
| Interface| Common examples for interface protocols.|[using_axi_master](Interface/Memory/using_axi_master)<br> [using_axi_lite_with_user_defined_offset](Interface/Register/using_axi_lite_with_user_defined_offset)<br> [using_axi_stream_with_side_channel_data](Interface/Streaming/using_axi_stream_with_side_channel_data)<br> [unaligned_burst_rgb](Interface/Memory/manual_burst/unaligned_burst_rgb)|
| Modeling| The essentials for loops, arbitrary precision types and vectors. Condtional control of HLS Pragmas.|[variable_bound_loops](Modeling/variable_bound_loops)<br> [using_arbitrary_precision_arith](Modeling/using_arbitrary_precision_arith)<br> [using_vectors](Modeling/using_vectors)<br> [using_array_stencil_1d](Modeling/using_array_stencil_1d)<br> [conditional_control_of_pragmas](Modeling/conditional_control_of_pragmas)|
| Pipelining| Illustrating one of the most fundamental concept of HLS.|[hier_func](Pipelining/Functions/hier_func)<br> [pipelined_loop](Pipelining/Loops/pipelined_loop)|
| Task_Level_Parallelism| Dataflow and free running streams with ```hls::task```. Autorestart support in testbench.|[using_stream_of_blocks](Task_level_Parallelism/Control_driven/Channels/using_stream_of_blocks)<br> [autorestart](Task_level_Parallelism/Control_driven/Channels/autorestart)<br> [unique_task_regions](Task_level_Parallelism/Data_driven/unique_task_regions)<br>[using_directio_hs_in_tasks](Task_level_Parallelism/Data_driven/using_directio_hs_in_tasks)|
| Misc| Other examples such as the RTL blackbox flow.|[rtl_as_blackbox](Misc/rtl_as_blackbox)|
| Migration| Examples covering scripted and command-line migration flows to Vitis Unified IDE.|[tcl_scripts](Migration/tcl_scripts)<br> [python_scripts](Migration/python_scripts)<br> [vitis_unified_cli](Migration/vitis_unified_cli)|

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

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
