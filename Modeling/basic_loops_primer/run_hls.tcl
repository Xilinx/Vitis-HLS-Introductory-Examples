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

# Utility procedure to be called in multiple places
proc run_rest_of_the_flow {{hls_exec 2}} {

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
}

# Create a project
open_project -reset proj_test

# Add design files
add_files test.cpp
# Add test bench & files
add_files -tb test_tb.cpp
add_files -tb result.golden.dat

# Set the top-level function
set_top test

# ########################################################
# Create Solution 1
open_solution -reset solution1 
# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period 10

# Set variable to select which steps to execute
set hls_exec 2

csim_design
# Set any optimization directives
set_directive_pipeline -off test/LOOP_1
# End of directives
run_rest_of_the_flow 1
close_solution

# ########################################################
# Create Solution 2
open_solution -reset solution2 
# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period 10

# Set variable to select which steps to execute
set hls_exec 2

csim_design
# Set any optimization directives
set_directive_pipeline -off test/LOOP_1
set_directive_unroll test/LOOP_1
# End of directives

run_rest_of_the_flow 1
close_solution

# ########################################################
# Create Solution 3
open_solution -reset solution3 
# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period 10

# Set variable to select which steps to execute
set hls_exec 2

csim_design
# Set any optimization directives
set_directive_pipeline -off test/LOOP_1
set_directive_unroll -factor=2 test/LOOP_1
# End of directives

run_rest_of_the_flow 1
close_solution

exit
