############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project -reset manual_burst
set_top transfer_kernel
add_files example.cpp
add_files example.h
add_files -tb example_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "manual_burst" -flow_target vitis
set_part {xcvu11p-flga2577-1-e}
create_clock -period 10 -name default
config_interface -m_axi_alignment_byte_size 64 -m_axi_latency 64 -m_axi_max_widen_bitwidth 512
config_rtl -register_reset_num 3
source "directives.tcl"
csim_design
csynth_design
cosim_design -trace_level all
