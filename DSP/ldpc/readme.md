Description
===========

These Vitis HLS design examples illustrate instantiations of the AMD/Xilinx LogiCORE LDPC Decoder from the Vivado IP catalog.

The LDPC (Low-Density Parity-Check) decoder is a hardblock IP available in Versal Premium Series Gen 2 and Versal RF devices. These examples demonstrate how to integrate the LDPC hardblock IP for high-performance 5G and communications applications.

The LDPC C++ instantiation supports stream (hls::stream<>) access modes for input and output data variables.

```
     +--(ldpc_top)--------------------+
     |                                |
   ->+--(input)-->[ldpc_wrapper]--(output)--+-->
     |                                |
     +-------------------------------+
```

The design variations for our examples are referenced in the table below.

## LDPC Hardblock Decoder Examples

|         Design name         | Use Case | Decoder Type | Data Interface | Features |
| :-------------------------: | :------------------: | :----: | :-----: | :---------------: |
| ldpc_5g | 5G Communications | Standard LDPC | AXI-Stream | 5G NR compatible, bit/symbol decoding |
| ldpc_words | Word-oriented Processing | Standard LDPC | AXI-Stream | Word-level data processing, variable frame sizes |

## Design Files

- **ldpc_tb.cpp**: The testbench which calls the top LDPC decoder function
- **ldpc_top.cpp**: The top function "ldpc_top"
- **ldpc_top.h**: Header file containing data structure definitions
- **hls_versal_ldpc_tb.h**: LDPC-specific testbench utilities and helper functions
- **run.py**: Python script to run the flow (c_simulation, c_synthesis, co_simulation)
- **run_hls.tcl**: Tcl script to run the flow (c_simulation, c_synthesis, co_simulation)
- **README**: This readme file

## Running the examples

In a terminal, setup the Vitis tools, navigate to the example directory and run the commands:

### Using Python:
```
$ vitis -s run.py
```

To open the component in the Vitis Unified IDE after running the Python script:
```
$ vitis -w work_ldpc_<variant>
```

### Using Tcl:
Alternatively, a Tcl script is provided. Run this command (also creates a component that can be opened in Vitis):
```
$ vitis-run --mode hls --tcl run_hls.tcl
```

## Performance

### Co-simulation Results

For the LDPC decoder examples, the following metrics are observed:

**ldpc_5g:**
- C Simulation Status: PASS
- RTL Co-Simulation Status: PASS
- Average Latency: ~5200 clock cycles
- Frame Interval: 2050 clock cycles
- Peak Stream Depth: 68 samples
- Execution Time: ~4 minutes

**ldpc_words:**
- C Simulation Status: PASS
- RTL Co-Simulation Status: PASS
- Average Latency: ~5200 clock cycles
- Frame Interval: 2050 clock cycles
- Peak Stream Depth: 91 samples
- Execution Time: ~3m55s

### Pipelining Characteristics

Both LDPC examples demonstrate pipelined execution:
- Multiple frames can be processed concurrently
- Maximum concurrent frames: 3 (in-flight simultaneously)
- Pipeline efficiency: ~39% overlap ratio
- While one frame is being processed, the next frame can be injected

### Clock Constraint

The designs target:
- Clock Period: 2.0 ns (500 MHz)
- Target Device: Versal RF or Premium Series Gen 2 (xcvr1602-vsva2488-2HP-i-S)

## Beta Device Support

These examples use Xilinx beta device support and enable beta devices in the HLS flow:
- Both Tcl and Python scripts include `enable_beta_device *` directive
- Supports early access to new Versal RF and Premium devices

## Co-simulation with Waveform Tracing

The examples support detailed waveform analysis:
- VCD/WDB file generation enabled via `config_cosim -trace_level all` in Tcl
- Waveform database size: ~6MB per design
- Can be opened in Vivado for detailed signal inspection

## Key Observations

- **Consistent Performance**: Both variants maintain stable latency and throughput
- **Stream Processing**: Full AXI-Stream interface support with handshake signals
- **5G Ready**: Designed for 5G NR LDPC decoding applications
- **Scalability**: Frame interval (2050 cycles) enables high-throughput operation

## Additional Resources

- [Vitis High-Level Synthesis User Guide (UG1399)](https://docs.amd.com/r/en-US/ug1399-vitis-hls)
- [LogiCORE IP LDPC Decoder v1.1 PG503](https://docs.xilinx.com/r/en-US/pg503-ldpc-decoder)
- [Versal Architecture Manual](https://docs.xilinx.com/r/en-US/am004-versal-user-guide)

These examples have been tested with Vitis v2026.2 on Versal RF and Premium Series Gen 2 devices.
