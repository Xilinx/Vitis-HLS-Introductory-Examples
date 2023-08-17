# clkperiod is coming from caller tcl script which contains something like set clkperiod 2.5
if {![info exists clkperiod]} {
  set clkperiod 2.5
}
open_project -reset proj_clk${clkperiod}

add_files duc.cpp
add_files -tb duc_test.cpp

set_top duc
open_solution -reset solution1

set_part {xcku040-ffva1156-2-e}

create_clock -period ${clkperiod}

config_dataflow -default_channel fifo -fifo_depth 256

csim_design
csynth_design
cosim_design -rtl verilog
export_design -evaluate verilog

exit
