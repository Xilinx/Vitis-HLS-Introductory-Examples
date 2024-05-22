<img src="./Images/logo.gif" alt="logo" width="200"/>

# Basic examples for Vitis HLS

Copyright 1986-2022 Xilinx, Inc. All Rights Reserved. 
Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.

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
* Dataflow: Common examples that illustrate usage of different channels and topologies
* Pipelining: Common examples that illustrate pipeline pragma usage for loops and functions
* Interface: Common examples that illustrate the usage of the various modes and interface protocols
* Modeling: Math and DSP examples and other common use models/algorithms
* Misc: Other examples such as RTL blackbox in C++

## Running the examples
A Tcl file is provided:
* _run_hls.tcl_: Sets up the project and specifies what steps of the flow will be executed (by default only C simulation and C synthesis are run).  
  By changing the value of hls_exec it's possible to run C-RTL co-simulation and Vivado implementation

To run at the command line, navigate to the example directory, type:
`vitis_hls -f run_hls.tcl`

To load the design into the HLS GUI, "Open"->"Project file" and select the project directory
