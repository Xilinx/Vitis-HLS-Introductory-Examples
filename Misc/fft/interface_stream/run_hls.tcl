#
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
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
#

# Project settings
# Create a project
open_project -reset proj_intf_stream


# Add the file for synthsis
add_files fft_top.cpp

# Add testbench files for co-simulation
add_files -tb fft_tb.cpp
add_files -tb data/stimulus_00.dat
add_files -tb data/stimulus_01.dat
add_files -tb data/stimulus_02.dat
add_files -tb data/stimulus_03.dat
add_files -tb data/stimulus_04.dat
add_files -tb data/stimulus_05.dat
add_files -tb data/stimulus_06.dat
add_files -tb data/stimulus_07.dat
add_files -tb data/stimulus_08.dat
add_files -tb data/stimulus_09.dat
add_files -tb data/stimulus_10.dat
add_files -tb data/stimulus_11.dat
add_files -tb data/stimulus_12.dat
add_files -tb data/stimulus_13.dat
add_files -tb data/stimulus_14.dat
add_files -tb data/stimulus_15.dat
add_files -tb data/stimulus_16.dat
add_files -tb data/stimulus_17.dat
add_files -tb data/stimulus_18.dat
add_files -tb data/stimulus_19.dat
add_files -tb data/stimulus_00.res
add_files -tb data/stimulus_01.res
add_files -tb data/stimulus_02.res
add_files -tb data/stimulus_03.res
add_files -tb data/stimulus_04.res
add_files -tb data/stimulus_05.res
add_files -tb data/stimulus_06.res
add_files -tb data/stimulus_07.res
add_files -tb data/stimulus_08.res
add_files -tb data/stimulus_09.res
add_files -tb data/stimulus_10.res
add_files -tb data/stimulus_11.res
add_files -tb data/stimulus_12.res
add_files -tb data/stimulus_13.res
add_files -tb data/stimulus_14.res
add_files -tb data/stimulus_15.res
add_files -tb data/stimulus_16.res
add_files -tb data/stimulus_17.res
add_files -tb data/stimulus_18.res
add_files -tb data/stimulus_19.res

# Set top module of the design
set_top fft_top

# Solution settings
open_solution -reset solution1 -flow_target vivado

# Define technology 
set_part  {xcvu9p-flga2104-2-i}

# Set the target clock period
create_clock -period 3.3

# Set to 0: to run setup
# Set to 1: to run setup and synthesis
# Set to 2: to run setup, synthesis and RTL simulation
# Set to 3: to run setup, synthesis, RTL simulation and RTL synthesis
# Any other value will run setup only
set hls_exec 2

# Run C simulation
csim_design

# Set any optimization directives

# End of directives

if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
    cosim_design -rtl verilog
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
    cosim_design -rtl verilog -trace_level all

	export_design
} else {
	# Default is to exit after setup
	csynth_design
}
exit


