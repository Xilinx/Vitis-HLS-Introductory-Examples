#!/bin/sh
: <<COMMENT
Script to run Vitis v++ and vitis-run commands for csim, csynth and cosim
COMMENT

HLS_CONFIG=hls_config.cfg
WORK_DIR=tmp

if [ $# -ne 1 ]; then
    echo "Usage: ./run_vitis_commandline.sh <csim(1)/csynth+cosim(2)>"
    exit
fi

selection=$1

case $selection in
    1)
        echo "Running C Simulation using vitis-run"
        vitis-run --mode hls --csim --config $HLS_CONFIG --work_dir $WORK_DIR
        ;;
    2)
        echo "Running C Synthesis using v++"
        v++ --compile --mode hls --config $HLS_CONFIG --work_dir $WORK_DIR

        echo "Running Co-Simulation using vitis-run"
        vitis-run --mode hls --cosim --config $HLS_CONFIG --work_dir $WORK_DIR
        ;;
    *)
        echo "Invalid input"
        exit
        ;;
esac

