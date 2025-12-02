Description
===========

These Vitis HLS design examples illustrate instantiations of the AMD/Xilinx LogiCORE FFT from the Vivado IP catalog.

The code micro-architecture in v2025.2 no longer requires data mover processes. A single process, the call to the FFT itself is sufficient.

```
     +--(fft_top)--------------------+
     |                               |
   ->+--(xn)-->[myfftwrapper]--(xk)--+-->
     |                               |
     +-------------------------------+
```

The FFT C++ instantiation supports arrays or streams (hls::stream<>) access modes for the variables "xn" and "xk" in the above diagram.
In the four example designs, the top level function arguments have the same data types as the internal variables. 

The design variations for our examples are referenced in the table below.


|         Design name         | Top level interfaces | FFT length |       Data types        | Super sample rate |
| :-------------------------: | :------------------: | :----:|:---------------------: | :---------------: |
|       interface_array       |        array         | 1024 | 16-bit (fixed point)   |        no         |
|      interface_stream       |        stream        | 1024 | 16-bit (fixed point)   |        no         |
| interface_array_float_ssr2  |        array         | 1024 | 32-bit (floating-point) |         2         |
| interface_stream_float_ssr2 |        stream        | 1024 | 32-bit (floating-point) |         2         |

These examples have been tested with Vitis v2025.2.

Design Files
============

- **data**: Directory with stimuli and results for the testbench (design without ssr)
- **data-ssr2, data-ssr4**: Directories with stimuli and results for the testbench (designs with ssr)
- **fft_tb.cpp**: The testbench which calls the top FFT function 20 times
- **fft_top.cpp**: The top function "fft_top"
- **fft_top.h**: Header file
- **run.py**: Python script to run the flow (csimulation, csynthesis, cosimulation, implementation)
- **README**: Readme file which refers to this readme.md

Running the examples
====================
In a terminal, setup the Vitis tools, navigate to the example directory and run the commands: 
```
$ vitis --source run.py
```
To open the component in the Vitis Unified IDE after running the Python script:
```
$ vitis -w wspace
```
To open the HLS component in Vitis Unified IDE after running the Python script:
```
$ vitis -w wspace
```

Performance
===========

## Throughput
After running the design example, you can check the throughput in the GUI by opening the co-simulation report and verifying the II numbers for the top-level function fft_top which will be about 1025 clock cycles without super sample rate and between 512 and 518 for the super sample data rate of 2 which is expected for the 1024 point FFT; you can also open the timeline trace viewer in Vitis or automatically launch the Vivado waveform if you enable the dump trace option.
On the command line you can collect the output of the latency report, for example 
```
$ find . -name lat.rpt | xargs grep -H THROUGHPUT | sed 's|^\./\([^/]*\)/.*:\(.*\)|\1: \2|'
interface_array: $MAX_THROUGHPUT = "1025"
interface_array: $MIN_THROUGHPUT = "1025"
interface_array: $AVER_THROUGHPUT = "1025"
interface_stream: $MAX_THROUGHPUT = "1025"
interface_stream: $MIN_THROUGHPUT = "1025"
interface_stream: $AVER_THROUGHPUT = "1025"
interface_array_float_ssr2: $MAX_THROUGHPUT = "518"
interface_array_float_ssr2: $MIN_THROUGHPUT = "512"
interface_array_float_ssr2: $AVER_THROUGHPUT = "516"
interface_stream_float_ssr2: $MAX_THROUGHPUT = "518"
interface_stream_float_ssr2: $MIN_THROUGHPUT = "513"
interface_stream_float_ssr2: $AVER_THROUGHPUT = "516"
```

## Place & Route Implementation Results

The following table shows the resource utilization and timing results from Vivado Place & Route implementation for all FFT design variants. All designs use a 2ns clock constraint (500 MHz target frequency).

| Design Name                 |  LUT |   FF |  DSP | BRAM | URAM |  SRL | Target (ns) | Post-Synthesis (ns) | Post-Route (ns) | Timing Met |
| :-------------------------- | ---: | ---: | ---: | ---: | ---: | ---: | ----------: | ------------------: | --------------: | :--------: |
| interface_array             | 3498 | 4345 |    8 |    3 |    0 | 1017 |       2.000 |               1.991 |           1.631 |     ✓      |
| interface_stream            | 3531 | 4361 |    8 |    3 |    0 | 1017 |       2.000 |               1.991 |           1.700 |     ✓      |
| interface_array_float_ssr2  | 7462 | 6482 |   58 |    8 |    0 | 2625 |       2.000 |               2.144 |           1.918 |     ✓      |
| interface_stream_float_ssr2 | 7470 | 6477 |   58 |    8 |    0 | 2625 |       2.000 |               2.144 |           1.984 |     ✓      |

### Key Observations

- **Integer vs Floating-point**: The examples with floating-point and super sample rate (SSR=2) require more resources as expected: 
  - ~2x more LUTs and FFs (16-bit integer versus 32-bit floating-point)
  - ~2.5x more BRAMs and SRLs (same comment as above)
  - ~7x more DSPs (in floating-point mode, the SSR is 2 and the DSP are also used for the adders)

- **Array vs Stream interfaces**: Minimal resource differences between array and streaming interfaces within the same data type category

- **Super Sample Rate (SSR)**: The SSR2 implementations trade area for throughput, achieving roughly 2x throughput improvement

- **Timing**: All four examples meet the 2ns (500 MHz) timing constraint in Vivado with comfortable margins

## Frequency
The constraint is 2ns (i.e. 500 MHz) and the device target is a Versal device (xcvc1902-vsva2197-2MP-e-S). Using the export flow, we can check what frequency is achieved after implementation option in Vivado. The achived frequency will vary depending on the FFT options and the target device and speed grade.