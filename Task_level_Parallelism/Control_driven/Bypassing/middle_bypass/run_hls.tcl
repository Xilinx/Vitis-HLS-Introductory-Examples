
#Solution -1 shows the problem 
open_project input_bypass_prob -reset
set_top dut
add_files dut.cpp
add_files -tb test_dut.cpp
open_solution "solution1"
set_part {xqku115-rlf1924-1-i}
create_clock -period 10 -name default
csim_design
csynth_design
cosim_design

#solution-2 to the input bypassing task 
open_project input_bypass_sol -reset
set_top dut
add_files dut_sol.cpp
add_files -tb test_dut.cpp
open_solution "solution1"
set_part {xqku115-rlf1924-1-i}
create_clock -period 10 -name default
csim_design
csynth_design
cosim_design

