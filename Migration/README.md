In this example, recommended scripted and command line flows are provided for migration to the Vitis Unified IDE for HLS development.

# Migration Paths
||Path|Usage|Notes|Relevant files|
|-|-|-|-|-|
|1| Tcl script| ```vitis-run --mode hls --tcl <tcl_script>```|```open_component``` reaplces ```open_project``` and ```open_solution```|tcl_scripts/run_vitis_unified.tcl|
|2| Vitis Unified Python script|```vitis -s <python_script>```|- configuration inside the script<br>- configuration in a file|run.py<br> hls_config.cfg (optional)|
|3| Vitis Unified Command Line<br> (v++, vitis-run)|C Simulation: ```vitis-run --mode hls --csim --config <config_file> --work_dir <work_dir>```<br> C Synthesis: ```v++ --compile --mode hls --config <config_file> --work_dir <work_dir>```<br> Co-Simulation: ```vitis-run --mode hls --cosim --config <config_file> --work_dir <>``` |HLS config file provided by the user|hls_config.cfg<br> run_vitis_commandline.sh|

# Common source files
- pointer_basic.h
- pointer_basic.cpp
- pointer_basic_test.cpp
- result.golden.dat
- README

# Online Documentation
[Migrating-from-Vitis-HLS-to-the-Vitis-Unified-IDE](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Migrating-from-Vitis-HLS-to-the-Vitis-Unified-IDE)

[Tcl-to-Config-File-Command-Map](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Tcl-to-Config-File-Command-Map)

