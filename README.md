<img src="./images/logo.gif" alt="logo" width="200"/>

# Basic examples for Vitis HLS

Copyright 2020 Xilinx, Inc.

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

Each includes code sources for top function and testbench, a README, Tcl files.  They are organized in categories denoted by the prefix of the directory in which they reside:
* **_Coding_**: Common coding styles for pointers, loops, arrays, etc...
* **_Algorithm_**: Math and DSP examples
* **_Interface_**: Manage block connectivity to other blocks
* **_Misc._**: Other examples such are RTL blackbox in C++

## New examples in v2020.2
* Vector data types with [](./coding_vectorized)
* ECC memories [](./memory_ecc_flags)
* Free running pipeline [](./coding_free_running_pipeline)

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
