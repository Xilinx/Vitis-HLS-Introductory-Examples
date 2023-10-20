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

open_project proj
set_top stable_pointer
add_files dut.cpp 
add_files -tb dut_tb.cpp 
open_solution "solution1" -flow_target vitis
set_part zynq
create_clock -period 3.125 -name default
csim_design 

# Set variable to select which steps to execute
set hls_exec 2

csim_design
if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design -enable_tasks_with_m_axi 
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design -enable_tasks_with_m_axi -rtl verilog -trace_level all
	export_design
} else {
	# Default is to exit after setup
	csynth_design
}

exit
