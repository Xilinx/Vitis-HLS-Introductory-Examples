Description
===========

These Vitis HLS design examples illustrate instantiations of the AMD Versal RF FFT hard IP block using the `hls_vrf_fft.h` library.

The FFT is a hardened block of the Versal RF devices: it does not use DSP resources of the programmable logic and Vitis HLS does not schedule its inside. A single process, the call to the FFT itself (`hls::vrf_fft::fft<config>`), is sufficient. The FFT C++ instantiation uses `hls::stream<>` arguments for the input and output data. The frame control values (point size, direction and scaling factor) are provided with the input samples.

```
     +--(fft_top)--------------------+
     |                               |
   ->+--(xn)-->[ vrf_fft::fft ]--(xk)+-->
     |                               |
     +-------------------------------+
```

The design variations for our examples are referenced in the table below.

|     Design name     | Top level interfaces | FFT length |         Data types           | Samples per clock | Output order | Scaling |
| :-----------------: | :------------------: | :--------: | :--------------------------: | :---------------: | :----------: | :-----: |
|    vrf_fft_base     |        stream        |    2048    | 18-bit in, 18-bit out        |         1         | bit-reversed |   yes   |
|   vrf_fft_fwd_inv   |        stream        |     32     | 16-bit in, 18-bit (forward) and 18-bit in, 31-bit out (inverse) |         1         | natural (forward), bit-reversed (inverse) | forward only |

- **vrf_fft_base** is the simplest instantiation. The testbench sends three frames, each one a complex tone in a different frequency bin, with the forward transform and a scaling factor of 1/2048. The output is in bit-reversed order, and the testbench uses `getIndex()` to put the bins in natural order before comparing with the expected result (a single non-zero bin of amplitude 0.5).
- **vrf_fft_fwd_inv** uses two hard FFT instances with different configurations in the same design: a scaled forward FFT with natural-order output, followed by an unscaled (full precision) inverse FFT. The forward FFT is scaled by 1/N, so the output of the design is equal to its input. The testbench compares the output with the 32 input samples read from a file.

These examples target a Versal RF device (xcvr1602-vsva2488-2HP-i-S-es1). The Versal RF FFT is not available on other devices.

Design Files
============

- **data**: Directory with the stimulus for the testbench (vrf_fft_fwd_inv only; the testbench of vrf_fft_base generates its stimulus)
- **fft_tb.cpp**: The testbench, which calls the top FFT function
- **fft_top.cpp**: The top function "fft_top"
- **fft_top.h**: Header file with the configuration of the FFT (`hls::vrf_fft::params_t`)
- **run.py**: Python script to run the flow (csimulation, csynthesis, cosimulation)
- **run_hls.tcl**: Tcl script to run the flow (csimulation, csynthesis, cosimulation, implementation)
- **README**: Readme file

Running the examples
====================
In a terminal, setup the Vitis tools, navigate to the example directory and run the commands:
```
$ vitis --source run.py
```
To open the component in the Vitis Unified IDE after running the Python script:
```
$ vitis -w work_<testcase>
```
As an alternative to Python, a Tcl script is provided. Run this command (also creates a component that can be opened in Vitis):
```
$ vitis-run --mode hls --tcl run_hls.tcl
```

Frame control and data format
=============================

The point size, the direction and the scaling factor of each frame are set with member functions of the input data type, on the samples of the frame. The testbenches set them on every sample, as in the examples:

```
in.setPointSize(11);       // log2 of the number of points: 11 = 2048 points
in.setDirection(true);     // true: forward FFT, false: inverse FFT
in.setScalingFactor(4);    // raw value of the scaling factor (only when FFT_SCALING is true)
in.tlast = last_sample_of_frame;
```

- The scaling factor is an unsigned fixed-point number in UFIX_13_13 format: the factor is the raw value divided by 8192 (raw values 0 to 8191). In the examples the raw value is chosen so that the scaling factor is 1/N, which keeps the output in the range of the input.
- Each call of the `fft` function in C simulation processes one frame: `FFT_LENGTH / SAMPLE_RATE` input words, with TLAST on the last word.
- When `FFT_REORDER` is false the output is in bit-reversed order, and `getIndex(j)` returns the frequency bin of sample `j` of an output word.
- The data types of the output are given by `output_t::data_out_t` (ap_fixed<18,1> for an 18-bit output with an 18-bit input, ap_fixed<31,14> for a 31-bit output with an 18-bit input).

Performance
===========

## Throughput
After running the design example, you can check the throughput in the GUI by opening the co-simulation report and verifying the II numbers for the top-level function fft_top. The FFT processes one sample per clock, so for vrf_fft_base the interval is expected to be close to the number of points of the transform (about 2048 cycles) when frames are sent back to back.
