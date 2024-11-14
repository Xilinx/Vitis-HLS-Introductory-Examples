#
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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
open_component -reset proj -flow_target vivado
set_top adder_top
add_files simple_adder.h
add_files adder_top.h
add_files adder_top.cpp
add_files -tb adder_tb.cpp 
set_part {xc7v585t-ffg1761-2}
create_clock -period 5 -name default

csim_design

csynth_design
	
cosim_design 


exit
