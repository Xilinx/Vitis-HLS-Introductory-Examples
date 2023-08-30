############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project maxi
set_top increment
add_files increment.cpp -cflags "-DHLS_TASK_ALLOW_NON_STREAM_ARGS -Wno-unknown-pragmas"
add_files -tb dut_tb.cpp -cflags "-DHLS_TASK_ALLOW_NON_STREAM_ARGS -Wno-unknown-pragmas"
open_solution "solution1" -flow_target vitis
set_part {xcvu11p-flga2577-1-e}
create_clock -period 10 -name default
config_cosim -tool xsim -trace_level all
#source "./maxi/solution1/directives.tcl"
csim_design
csynth_design
config_cosim -enable_tasks_with_m_axi
cosim_design -enable_tasks_with_m_axi -rtl verilog -trace_level all
export_design -format ip_catalog
