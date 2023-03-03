#!/bin/bash

# the script assumes Vitis HLS is setup and will launch the tool in a separate xterm
#
# to run all versions :
# $ ./run_all.bash 2022.2/
#
# to run only one version :
# $ ./run_all.bash 2022.2/fft_single_interface_array_internal_array
#

if test $# -ne 1 || ! test -d $1 ; then
    echo "needs only 1 argument : the version of the code which is the directory name like 2022.2 or 2023.1, but got '$@'"
    exit
fi

TCLFILES=`find $1 -type f -name run_hls.tcl`

echo "will run using $TCLFILES"

timeout=90

for tcl in $TCLFILES ; do
    dir=`dirname $tcl`
    src=`basename $tcl`
    printf "%-60s -> %14s\n" $dir $src
    (cd $dir ; xterm -title "$dir" -e "vitis_hls -f $src ; read -p \"pausing execution for $timeout seconds\" -t $timeout" ; echo "***** done $dir") & echo -ne ""
done

