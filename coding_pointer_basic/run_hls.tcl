#
# Copyright 2021 Xilinx, Inc.
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
open_project -reset proj_pointer_basic

# Add design files
add_files pointer_basic.c
# Add test bench & files
add_files -tb pointer_basic_test.c
add_files -tb result.golden.dat

# Set the top-level function
set_top pointer_basic

# ########################################################
# Create a solution
open_solution -reset solution1
# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period 4

# Source x_hls.tcl to determine which steps to execute
source x_hls.tcl
csim_design
# Set any optimization directives
# End of directives

# cosim_design option -trace_level all is used to create a VCD waveform file
if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design -trace_level all
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design -trace_level all
	export_design
} else {
	# Default is to exit after setup
	csynth_design
}

# ########################################################
# Create a solution
open_solution -reset solution2
# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period 4

# Source x_hls.tcl to determine which steps to execute
source x_hls.tcl

# Set any optimization directives
set_directive_interface -mode ap_hs "pointer_basic" d
# End of directives

# cosim_design option -trace_level all is used to create a VCD waveform file
if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design -trace_level all
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design -trace_level all
	export_design
} else {
	# Default is to exit after setup
	csynth_design
}


exit

