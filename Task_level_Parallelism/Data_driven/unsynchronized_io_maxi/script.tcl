############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project proj
set_top stable_pointer
add_files dut.cpp 
add_files -tb dut_tb.cpp 
open_solution "solution1" -flow_target vitis
set_part zynq
create_clock -period 3.125 -name default
csim_design 
csynth_design
cosim_design -enable_tasks_with_m_axi -rtl verilog -trace_level all
