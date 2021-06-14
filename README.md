<img src="./images/logo.gif" alt="logo" width="200"/>

# Basic examples for Vitis HLS

Copyright 2021 Xilinx, Inc.

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

> NOTE: If you are looking for a Vitis HLS tutorial rather than a list of examples, go here:  
> https://github.com/Xilinx/Vitis-Tutorials/tree/master/Getting_Started/Vitis_HLS

Each of the following examples includes code source for top function and testbench, a README, Tcl files.  They are organized in categories denoted by the prefix of the directory in which they reside:
* **_Coding_**: Common coding styles for pointers, loops, arrays, etc...
* **_Algorithm_**: Math and DSP examples
* **_Interface_**: Manage block connectivity to other blocks
* **_Misc._**: Other examples, e.g. RTL blackbox in C++

## Running the examples

Two Tcl files are provided:
* _x_hls.tcl_: Short script sourced in run_hls.tcl to specify the steps of the flow which will be executed (by default only C simulation and C synthesis are run).  By changing the value of the variable hls_exec it's possible to run C-RTL co-simulation and launch a Vivado implementation
* _run_hls.tcl_: Sets up the project and sources x_hls.tcl mentioned above

To run at the command line, navigate to the example directory, type:
`vitis_hls -f run_hls.tcl`

To load an example design into the Vitis HLS GUI:
* From the "Welcome" screen ("Help"->"Welcome..."), click on "Clone Examples" and clone this repository
* Next, in the "Git Repositories" window pane in the lower left of the GUI, expand the "Working Tree"
* Right click on the example of interest and select "Run and Open HLS project"

## List and description of the examples
Examples | Description
---------|------------
[algorithm_2D_convolution_linebuffer](./algorithm_2D_convolution_linebuffer)|2D convolution implemented using `hls::streams` and a line buffer to conserve resources.
[algorithm_fir_systolic_scalar](./algorithm_fir_systolic_scalar)| systolic fir filter
[algorithm_fixed_point_sqrt](./algorithm_fixed_point_sqrt)| square root function based on fixed-point data types
[algorithm_floating_mult_power_of_2](./algorithm_floating_mult_power_of_2)| Advanced floating-point power of 2 with bounds checking and defining an helper typedefs to allow easy parsing of the IEEE-754 floating point fields
[algorithm_template_fir_filter](./algorithm_template_fir_filter)| FIR filter using C++ template parameters
[algorithm_window_function_fixed_point](./algorithm_window_function_fixed_point)| Using fixed-point data types. Window convolution using rectangle, Hann, Hamming and gaussian window.
[algorithm_window_function_float](./algorithm_window_function_float)| Using floating point data types. Window convolution using rectangle, Hann, Hamming and gaussian window.
[coding_arbitrary_precision_arith](./coding_arbitrary_precision_arith)| Defining arbitrary precision variables through the `ap_int.h` library
[coding_arbitrary_precision_casting](./coding_arbitrary_precision_casting)| Casting to/from arbitrary precision data types defined through `ap_int.h`
[coding_C++_templates](./coding_C++_templates)| C++ templatized struct to implement a tail-recursion Fibonacci algorithm
[coding_dataflow_rewind](./coding_dataflow_rewind)| Task parallelism with the `dataflow` pragma. Also uses `pipeline` with `rewind` and `unroll` with `factor`
[coding_fixed_point](./coding_fixed_point)| Fixed-point arithmetic
[coding_free_running_pipeline](./coding_free_running_pipeline) | Alternate control for loops and functions, potentially reducing control logic fanout in Vivado
[coding_function_instantiate](./coding_function_instantiate) | Illustrates the use of the pragma `function_instantiate` to optimize independently instances of a function based on its inputs
[coding_hierarchy_func](./coding_hierarchy_func)|An example of adding files as testbench and design files.
[coding_hierarchy_func2](./coding_hierarchy_func2)|An example of adding files as test bench and design files. An example of synthesizing a lower-level block in the hierarchy.
[coding_hierarchy_func3](./coding_hierarchy_func3)|An example of combining test bench and design functions into the same file.
[coding_hierarchy_func4](./coding_hierarchy_func4)|Using the pre-defined macro `__SYNTHESIS__` to prevent code being synthesized. Only use the `__SYNTHESIS__` macro in the code to be synthesized. Do not use this macro in the test bench,because it is not obeyed by C simulation or C RTL co-simulation.
[coding_loop_functions](./coding_loop_functions)|Converting loops into functions for parallel execution.
[coding_loop_imperfect](./coding_loop_imperfect)|An imperfect loop example.
[coding_loop_labels](./coding_loop_labels)|Using labels to tag loops and helps find them in the reports.  Note that without a label Vitis HLS still assigns a machine generated label in the reports
[coding_loop_max_bound_trick](./coding_loop_max_bound_trick)|Using a maximum bounds to allow loops be unrolled.
[coding_loop_perfect](./coding_loop_perfect)|Using a maximum bounds to allow loops be unrolled.
[coding_loop_pipeline](./coding_loop_pipeline)|Example of loop pipelining.
[coding_loop_sequential](./coding_loop_sequential)|Two accumulators described one after another in 2 loops.  Try making them execute in parallel using `dataflow`
[coding_loop_sequential_assert](./coding_loop_sequential_assert)|Same example as above but using `assert` statements.
[coding_loop_sequential_dataflow](./coding_loop_sequential_dataflow)| Loop and dataflow (task parallelism)
[coding_loop_variable_bound](./coding_loop_variable_bound)|A loop with variable bounds.
[coding_malloc_removed](./coding_malloc_removed)|Example on removing mallocs from the code.
[coding_pointer_arith](./coding_pointer_arith)| Pointer and arithmetic
[coding_pointer_basic](./coding_pointer_basic)| Basic use of pointer
[coding_pointer_cast_native](./coding_pointer_cast_native)| Pointer casting
[coding_pointer_double](./coding_pointer_double)| Double pointer
[coding_pointer_multi](./coding_pointer_multi)| Example in which the assignment of a pointer depends on some variable test
[coding_pointer_stream_better](./coding_pointer_stream_better)| Pointer access on `volatile` inputs without specifying the index
[coding_pointer_stream_good](./coding_pointer_stream_good)| Same as above but updating the point position
[coding_remerge_ii2to1](./coding_remerge_ii2to1)| Coding trick to recoup II of 1 based on 2 sources with II of 2
[coding_vectorized](./coding_vectorized)| Load-compute-store based on vector data types using the vector_size attribute
[interface_axi_lite](./interface_axi_lite)| AXI-4 Lite on C ports
[interface_axi_master](./interface_axi_master)| AXI-4 Master memory mapped on C ports
[interface_axi_stream_complex_tlast](./interface_axi_stream_complex_tlast) | Uses TLAST and sends/receives complex data types.
[interface_axi_stream_to_master](./interface_axi_stream_to_master) | Going from stream to master while ensuring burst transfers. Also uses `dataflow`.
[interface_axi_stream_no_side_channel_data](./interface_axi_stream_no_side_channel_data)| Regular stream without sideband signals
[interface_axi_stream_side_channel_data](./interface_axi_stream_side_channel_data)| Adding side-band signals for streams. Uses `ap_axi_sdata.h`.
[interface_hls_stream](./interface_hls_stream)| stream on the C ports
[memory_bottleneck](./memory_bottleneck)| Array accesses that prevent full throughput
[memory_bottleneck_resolved](./memory_bottleneck_resolved)| Offers a solution to memory_bottleneck
[memory_ecc_flags](./memory_ecc_flags)| Setting up a memory (an UltraRAM in this example) to generate error correction flags for 1 bit corrected errors and 2 bits uncorrected errors.  This is accomplished via the `BIND_STORAGE`pragma.
[memory_rom_coef_filter](./memory_rom_coef_filter)| Loading filter coefficients into a memory
[memory_rom_lookup_table](./memory_rom_lookup_table)| Using an array filled with constants as a lookup
[memory_rom_lookup_table_math](./memory_rom_lookup_table_math)| Presetting a memory with pre-calculated content
[memory_ultraram](./memory_ultraram)| Inference of an UltraRAM block (URAM) which is a 288 k-bit memory available in UltraScale+ and Versal devices.  Uses the `BIND_STORAGE` and applies the `DEPENDENCE` pragma to obtain an II of 1.
[misc_rtl_as_blackbox](./misc_rtl_as_blackbox)| A user supplied Verilog module is swapped for a C function.  That user RTL code then infers in Vivado a DSP block in its intrinsic SIMD mode using 4 adders.
