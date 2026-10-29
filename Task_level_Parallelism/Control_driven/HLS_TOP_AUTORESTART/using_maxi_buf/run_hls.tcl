#
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
open_component -reset my_dut -flow_target vivado

# Add design files
add_files test.cpp
# Add test bench & files
add_files -tb test_tb.cpp

# Set the top-level function
set_top my_dut::top

# ########################################################
# Create a solution
# Define technology and clock rate
set_part {virtex7}
create_clock -period 4 -name default

# Set variable to select which steps to execute
# 0 = csim only (live offset updates are visible here)
# 1 = csim + csynth
# 2 = csim + csynth + cosim
set hls_exec 0

csim_design

if {$hls_exec == 1} {
	csynth_design
} elseif {$hls_exec == 2} {
	csynth_design
	cosim_design
}

exit
