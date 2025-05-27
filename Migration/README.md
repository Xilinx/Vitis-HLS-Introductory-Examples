In this example, recommended scripted and command line flows are provided for migration to the Vitis Unified IDE for HLS development.

# Recommended Scripted and Command-Line Migration Flows
||Path|Usage|Notes|Relevant files|
|-|-|-|-|-|
|1| Tcl script| ```vitis-run --mode hls --tcl <tcl_script>```|```open_component``` replaces ```open_project``` and ```open_solution```|tcl_scripts/run_vitis_unified.tcl|
|2| Vitis Unified Python script|```vitis -s <python_script>```|Two ways:<br>- configuration inside the script<br>- configuration in a file|python_scripts/run.py<br> python_scripts/hls_config.cfg, python_scripts/run_with_external_config.py|
|3| Vitis Unified Command Line Interface|C Simulation: ```vitis-run --mode hls --csim --config <config_file> --work_dir <work_dir>```<br> C Synthesis: ```v++ --compile --mode hls --config <config_file> --work_dir <work_dir>```<br> Co-Simulation: ```vitis-run --mode hls --cosim --config <config_file> --work_dir <work_dir>``` |HLS config file provided by the user|vitis_unified_cli/hls_config.cfg<br> vitis_unified_cli/Makefile|

# Common source files
- pointer_basic.h
- pointer_basic.c
- pointer_basic_test.c
- result.golden.dat
- README.md

# Online Documentation
[Migrating-from-Vitis-HLS-to-the-Vitis-Unified-IDE](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Migrating-from-Vitis-HLS-to-the-Vitis-Unified-IDE)

[Tcl-to-Config-File-Command-Map](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Tcl-to-Config-File-Command-Map)

