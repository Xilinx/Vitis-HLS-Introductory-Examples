Description
===========

These Vitis HLS design examples illustrate instantiations of the AMD Versal LDPC Decoder hard IP block using the `hls_versal_ldpc.h` library.

The LDPC decoder is a hardened block: Vitis HLS does not schedule its inside. A single process, the call to the decoder itself (`hls::versal_ldpc::ldpc_decode<config>`), is sufficient. The decoder is configured at compile time with a struct that inherits from `hls::versal_ldpc::params_t`, using the parameter names and default values of the IP in Vivado. Its interface is a set of `hls::stream<>` arguments, one for each AXI4-Stream channel of the IP.

```
     +--(ldpc_top)------------------------------+
     |                                           |
   ->+--(ctrl_s, din_s)-->[ ldpc_decode ]--(status_s, dout_s)-->
     |                                           |
     +-------------------------------------------+
```

The words streams (din_words_s, dout_words_s) exist only when the corresponding words configuration is not fixed.

The design variations for our examples are referenced in the table below.

|  Design name  | Top level interfaces | Standard | DIN words configuration | DOUT words configuration | Lanes (DIN / DOUT) | AXI4-Stream channels |
| :-----------: | :------------------: | :------: | :---------------------: | :----------------------: | :----------------: | :------------------: |
|   ldpc_5g     |        stream        |  5G NR   |          fixed          |          fixed           |       1 / 1        |          4           |
|  ldpc_words   |        stream        |  5G NR   |     per transaction     |        per block         |       1 / 1        |          6           |

- **ldpc_5g** is the simplest instantiation. The testbench sends two blocks of a 5G NR code (base graph 1, 46 parity rows, selected by the geometry fields of the control packet). Each block is the all-zero codeword with random bit errors on the input LLRs. The testbench checks that the decoder reports a parity pass, that all the decoded information bits are zero, and that TLAST and TKEEP are correct on the output.
- **ldpc_words** uses the words sidebands of the IP: the number of LLRs of each input beat (24) is sent on din_words_s, and the number of bytes of each output beat (20) is sent on dout_words_s. The testbench packs and unpacks the blocks with these numbers, so the block decodes correctly only if the library honors them.

These examples target a Versal RF device (xcvr1602-vsva2488-2HP-i-S-es1). The Versal LDPC Decoder is also available on some Versal Premium Series Gen 2 devices.

Design Files
============

- **hls_versal_ldpc_tb.h**: Optional helper functions for the testbench (not part of the library): `din_pack` packs the LLRs of a block into din beats and drives the din_words stream, `dout_words_drive` drives the dout_words stream, and `dout_unpack` unpacks the dout beats and checks TKEEP and TLAST
- **ldpc_tb.cpp**: The self-checking testbench, which calls the top LDPC function once for each block
- **ldpc_top.cpp**: The top function "ldpc_top"
- **ldpc_top.h**: Header file with the configuration of the decoder (`hls::versal_ldpc::params_t`)
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

Blocks, control and status packets
==================================

- Each call of `ldpc_decode` in C simulation decodes one block. The testbench writes the control packet, the LLRs of the block (with TLAST on the last din beat), and the words packets if any, and then calls the top function.
- The input values are LLRs in signed fixed-point format Fix(8,2) (`in_llr_t`: 8 bits, 2 of them fractional). A bit equal to 0 gives a negative LLR in the examples (-4).
- The control packet (`ctrl_t`) and the status packet (`status_t`) have `set_` and `get_` accessors for each field. For the 5G standard, the code is selected by the fields z_j, z_set, bg and mb. `query_code_params<config>(ctrl, N, K)` returns the number of LLRs (N) and of information bits (K) of the selected code.
- The examples set the algorithm fields of the control packet (algorithm, alg_mod, nsat_bits and max_schedule) to the default values of the static parameters. The IP uses these fields for a block when the corresponding `*_Source` parameter is CTRL.
- With hard-decision output (`set_hard_op(true)`), each byte of dout holds 8 decoded bits, and the number of output bits is in the num_op field of the status packet.
- The order of arrival of the status and dout packets must not be assumed.

Performance
===========

## Throughput
To measure the sustained throughput, use a top function with the ap_ctrl_none protocol so that co-simulation sends the blocks back to back, and send one more block at the end to flush the pipeline. The examples are functional examples and do not measure throughput.
