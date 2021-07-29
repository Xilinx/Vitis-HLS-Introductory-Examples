############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project input_bypass_prob
set_top dut
add_files dut.cpp
add_files -tb test_dut.cpp
open_solution "solution1"
set_part {xqku115-rlf1924-1-i}
create_clock -period 10 -name default
#source "./input_bypass/solution1/directives.tcl"
csim_design
csynth_design
cosim_design -compiler clang -trace_level all
export_design -format ip_catalog
