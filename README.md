<img src="./images/logo.gif" alt="logo" width="200"/>

# Examples for Xilinx Vitis HLS

## C/C++ synthesizable examples

Each includes code sources for top function and testbench, a README, Tcl files.  They are organized in categories denoted by the prefix of the directory in which they reside:
* **_Coding_**: Common coding styles for pointers, loops, arrays, etc...
* **_Algorithm_**: Math and DSP examples
* **_Interface_**: Manage block connectivity to other blocks
* **_Misc._**: Other examples such are RTL blackboxes
<details>
  <summary>Click for examples details!</summary>
  
  ## Coding
  1. Pointers
  2. Loops
     * Pipeline
     * Unroll
  ## Algorithm
  1. DSP
  2. Loops
     * Pipeline
     * Unroll
 ## Interface
  1. DSP
  2. Loops
     * Pipeline
     * Unroll
 ## Library
  1. DSP
  2. Loops
     * Pipeline
     * Unroll
</details>


## Running the examples
Two Tcl files are provided:
* _x_hls.tcl_: Specifies what steps of the flow will be executed (by default only C simulation and C synthesis are run).  By changing the value of hls_exec it's possible to run C-RTL co-simulation and Vivado implementation
* _run_hls.tcl_: Sets up the project and sources x_hls.tcl mentioned above

To run at the command line, navigate to the example directory, type:
`vitis_hls -f run_hls.tcl`

To load the design into the Vitis HLS GUI, "Open"->"Project file" and select the project directory
