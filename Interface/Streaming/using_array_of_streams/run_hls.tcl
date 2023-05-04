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
open_project -reset proj_array_of_streams

# The source file and test bench
add_files example.cpp
add_files -tb example_test.cpp

# Specify the top-level function for synthesis
set_top	dut

###########################
# Solution settings

# Create solution1
open_solution -reset solution1 -flow_target vitis 

# Define technology and clock rate
set_part  {xcvu9p-flga2104-2-i}
create_clock -period "400MHz"

csim_design
# Run Synthesis, RTL Simulation, RTL implementation and Exit
csynth_design
cosim_design
# Generate pcore
# export_design -format ip_catalog

exit


