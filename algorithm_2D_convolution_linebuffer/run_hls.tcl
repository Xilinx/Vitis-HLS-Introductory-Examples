#
# Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Create a project
open_project -reset proj_2D_convolution_with_linebuffer

# Add design files
add_files convolution.cpp 
# Add test bench & files
add_files -tb convolution_test.cpp

# Set the top-level function
set_top filter11x11_strm

# ########################################################
# Create a solution
open_solution -reset solution1
# Define technology and clock rate
set_part {xcvu9p-flga2104-2-i}
create_clock -period 6.66

# Source x_hls.tcl to determine which steps to execute
source x_hls.tcl
csim_design
# Set any optimization directives
# End of directives

if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design
	export_design
} else {
	# Default is to exit after setup
	csynth_design
}

exit

open_project loop_opts_prj

set_top filter11x11_strm

add_files convolution.cpp
add_files -tb convolution_tb.cpp

open_solution "solution1"
set_part {xcvu9p-flga2104-2-i}
create_clock -period 6.66 -name default

csim_design -clean
csynth_design
#cosim_design -trace_level none -rtl systemc
#export_design -format ip_catalog
exit