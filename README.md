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

## Running the examples
Two Tcl files are provided:
* _x_hls.tcl_: Specifies what steps of the flow will be executed (by default only C simulation and C synthesis are run).  By changing the value of hls_exec it's possible to run C-RTL co-simulation and Vivado implementation
* _run_hls.tcl_: Sets up the project and sources x_hls.tcl mentioned above

To run at the command line, navigate to the example directory, type:
`vitis_hls -f run_hls.tcl`

To load the design into the Vitis HLS GUI, from the "Welcome" screen ("Help"->"Welcome..."), click on "Open Example" and select the example project of interest
