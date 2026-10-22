#
# Copyright 2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
open_component -reset comp_ldpc_words -flow_target vivado

# Add design files
add_files ldpc_top.cpp
# Add testbench files (hls_versal_ldpc_tb.h is included by the testbench)
add_files -tb ldpc_tb.cpp

# Set the top-level function
set_top ldpc_top

# Define technology and clock rate (the Versal LDPC Decoder requires a Versal RF or a Versal Premium Series Gen 2 device)
set_part  xcvr1602-vsva2488-2HP-i-S-es1
create_clock -period 2.0

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
	# Default is to exit after running csynth
	csynth_design
}

exit
