# Create a project
open_project -reset proj

#Add design files
add_files example.cpp
#Add test bench & files
add_files -tb example_test.cpp

#Set the top-level function
set_top example

#Create a solution
open_solution -reset solution1
#Define technology and clock rate
set_part  {virtexu}
create_clock -period 3
config_flow -target vitis

csim_design
csynth_design	
cosim_design


