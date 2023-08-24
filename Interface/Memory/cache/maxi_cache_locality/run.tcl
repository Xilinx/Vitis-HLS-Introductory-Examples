set top dut
set src_file dut.cpp

open_project -reset proj

add_files $src_file
add_files -tb dut_tb.cpp
set_top $top

open_solution -reset solution1
set_part virtexuplus
create_clock -period "300MHz"
csim_design
csynth_design
cosim_design

exit
