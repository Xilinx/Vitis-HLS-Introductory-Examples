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
open_project -reset conv_filter_prj
set_top Filter2DKernel
add_files -tb hls_testbench.cpp  
add_files -tb cmdlineparser.cpp  
add_files -tb filter2d_sw.cpp    
add_files     filter2d_hw.cpp    
# ########################################################
# Create a solution
open_solution "solution"
set_part {xcku11p-ffva1156-1-e}
create_clock -period 3.33333333 -name default
config_flow -target vivado
# Run Synthesis, RTL Simulation, RTL implementation and Exit
# Set variable to select which steps to execute
set hls_exec 2

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
