# Examples for Xilinx Vitis HLS

## C/C++ synthesizable examples

Each includes code sources for top function and testbench, a README, Tcl files.  They are organized in categories denoted by the prefix of the directory in which they reside:
* _Coding_: Common coding styles for loops, arrays, etc...
* _Algorithms_: Math and DSP examples
* _Interface_: Manage block connectivity to other blocks
* _Libraries_: Using libraries included in HLS
* _Misc._: Other examples such are RTL blackboxes

## Running the examples
Two Tcl files are provided to help run the examples:
* _x_hls.tcl_: Specifies what steps of the flow will be executed (by default only C simulation and C synthesis are run
* _run_hls.tcl_: Sets up the project and sources x_hls.tcl mentioned above

To run at the command line, in a terminal type:
`vitis_hls -f run_hls.tcl`

To load the design into the Vitis HLS GUI, "Open"->"Project file" and select the project directory
