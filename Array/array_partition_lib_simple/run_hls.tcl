set my_top "array_partition_lib_simple"

open_component -reset component_$my_top -flow_target vivado

add_files $my_top.cpp
add_files -tb ${my_top}_test.cpp

set_top $my_top

set_part {xcvc1902-vsva2197-2MP-e-S}
create_clock -period 2ns

set hls_exec 2

csim_design

if {$hls_exec == 1} {
    csynth_design
} elseif {$hls_exec == 2} {
    csynth_design
    cosim_design
} else {
    csynth_design
    cosim_design
    export_design -flow impl
}

exit
