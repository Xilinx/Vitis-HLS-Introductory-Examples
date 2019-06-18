# Add pipeline directives to improve throughput/latency
set_directive_pipeline "qrf_alt/col_assign_q_i"
set_directive_pipeline "qrf_alt/col_assign_r_i"
set_directive_pipeline "qrf_alt/calc_rotations"
set_directive_pipeline "qrf_alt/update_r"
set_directive_pipeline "qrf_alt/update_q"
set_directive_pipeline "qrf_alt/assign_r_col_loop"
set_directive_pipeline "qrf_alt/assign_q_col_loop"
# Partion array and partially unroll loops that update those arrays to further improve throughput
set_directive_array_partition -type cyclic -factor 2 -dim 2 "qrf_alt" q_i
set_directive_array_partition -type cyclic -factor 2 -dim 2 "qrf_alt" r_i
set_directive_unroll -factor 2 "qrf_alt/update_r"
set_directive_unroll -factor 2 "qrf_alt/update_q"


