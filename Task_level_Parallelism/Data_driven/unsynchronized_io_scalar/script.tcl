############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project proj
set_top test
add_files mt.cpp -cflags "-DHLS_TASK_ALLOW_NON_STREAM_ARGS"
add_files -tb mt.cpp -cflags "-DHLS_TASK_ALLOW_NON_STREAM_ARGS -Wno-unknown-pragmas"
open_solution "solution1" -flow_target vitis
set_part {xc7v585t-ffg1761-2}
create_clock -period 5 -name default
config_export -vivado_clock 5
source "./proj/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -flow syn -rtl verilog -format ip_catalog
