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
* Appnotes: A DSP design, a legacy digital up converter appnote.
* Array: Show how to partition memory arrays.
* Interface: Common examples for interface protocols.
* Misc.: Other examples such as the RTL blackbox flow and the LogiCore FFT from Vivado.
* Modelling: The essentials for loops, arbitrary precision types and vectors.
* Pipelining: Illustrating one of the most fundamental concept of HLS.
* Task_Level_Parallelism: Dataflow and free running streams with hls::task.
* Vitis: Kernel examples for the software acceleration flow.

## Running the examples at the command line

* Using Vitis Unified IDE:
  - vitis-run --mode hls --tcl run_hls.tcl
* Using Vitis HLS (deprecated):
  - vitis_hls -f run_hls.tcl

Note that by default only C simulation and C synthesis will run. Change the value of hls_exec in the Tcl script to run co-simulation and Vivado implementation.

