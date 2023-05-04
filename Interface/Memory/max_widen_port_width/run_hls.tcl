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

# Create a project
open_project -reset project

#Add design files
add_files example.cpp
# Add test bench & files
add_files -tb example_test.cpp 

# Set the top-level function
set_top example

# ########################################################
# Create a solution
open_solution -reset solution -flow_target vivado 

# Define technology and clock rate
set_part {xcvu9p-flga2104-2-i}
create_clock -period "75MHz"

# Set any global configuration settings
config_interface -m_axi_max_widen_bitwidth 256
config_interface -m_axi_alignment_byte_size 64

csim_design
csynth_design
cosim_design

exit
