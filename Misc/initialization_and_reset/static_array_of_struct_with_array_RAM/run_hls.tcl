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

# ##################################################
# Utility procedures to be called in multiple places
# ##################################################
proc make_new_solution {{sol_name "solution1"}} {
    # Create the solution
    open_solution -reset $sol_name -flow_target vitis

    # Define technology and clock rate
    set_part  {xcvu9p-flga2104-2-i}
    create_clock -period 25
}

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

# ##################################################
# Main Entry: Create Project 
# ##################################################
open_project -reset proj

# Add design files
add_files test.cpp
# Add test bench & files
add_files -tb test_tb.cpp

# Set the top-level function
set_top test

# ########################################################
# Solution_A
# ########################################################
make_new_solution "solution_A"

csim_design

# Set variable to select which steps to execute
set hls_exec 2

run_rest_of_the_flow $hls_exec

close_solution

# ########################################################
# Solution_B
# ########################################################
make_new_solution "solution_B"

csim_design

# Set any optimization directives
set_directive_reset test ts
# End of directives

# Set variable to select which steps to execute
set hls_exec 2

run_rest_of_the_flow $hls_exec

close_solution

# ########################################################
# Solution_C
# ########################################################
make_new_solution "solution_C"

csim_design

# Set any optimization directives
config_rtl -reset state 
# End of directives

# Set variable to select which steps to execute
set hls_exec 2

run_rest_of_the_flow $hls_exec

exit

