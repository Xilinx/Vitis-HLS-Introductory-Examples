#!/bin/sh
# Script to run Vitis v++ and vitis-run commands for csim, csynth, cosim amd impl(implementation)

HLS_CONFIG="hls_config-for-commandline-use.cfg"
WORK_DIR="work_dir"
USAGE="\n Usage: $0 [h|help] [csim] [csynth] [cosim] [impl]\n   $0 will call vitis-run or v++ using the config file $HLS_CONFIG and generate the outputs in $WORK_DIR.\n"
COMMON_OPTIONS="--mode hls --config $HLS_CONFIG --work_dir $WORK_DIR"

if [ $# -eq 0 ]; then
    echo -ne $USAGE
    exit
fi

for selection in $* ; do
    case $selection in
        h | -h | --h | help | -help | --help)
            echo -ne "Help -- $USAGE"
            ;;
        csim)
            echo "Running C Simulation using vitis-run"
            vitis-run --csim    $COMMON_OPTIONS
            ;;
        csynth)
            echo "Running C Synthesis using v++"
            v++       --compile $COMMON_OPTIONS
            ;;
        cosim)
            echo "Running Co-Simulation using vitis-run"
            vitis-run --cosim   $COMMON_OPTIONS
            ;;
        impl)
            echo "Running Implementation using vitis-run"
            vitis-run --impl    $COMMON_OPTIONS
            ;;
        *)
            echo -ne "Invalid input: $selection -- $USAGE"
            ;;
    esac
done
