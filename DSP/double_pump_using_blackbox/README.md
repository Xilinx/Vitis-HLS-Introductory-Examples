# double_pump_using_blackbox

A double-clocking example. `dp_mac4` is a 4-lane int8 multiply-accumulate whose
multipliers run on a clock at **2x the base clock**, so 4 multipliers perform
8 int8 MACs per base-clock cycle. The double-clocked datapath is an RTL black box
called from HLS through a black-box JSON. A C model of the box is used for C
simulation.

```
lane l result = w[2l]*x[2l]  (2x phase 0)  +  w[2l+1]*x[2l+1]  (2x phase 1)     l = 0..3
```

Port layout: `In = {tag[7:0], W[63:0], X[63:0]}` (136 bits), return = `{tag, 4 x int32}`.
The tag is the iteration index. The box delays it along with the result. The
wrapper counts tag mismatches in `out[4]` and records the first mismatching
iteration in `out[5]`.

## Layout

| path | contents |
|---|---|
| `bb/dp_mac4.v` | RTL black box (base clock plus 2x clock) |
| `bb/dp_mac4.cpp` | C model for C simulation; bit-exact with the RTL |
| `bb/dp_mac4.json` | Black-box registration: ports, latency 5, 2x clock domain |
| `src/dp_top.{h,cpp}` | HLS top: `ap_memory` input, II=1 loop, tag check |
| `tb/dp_tb.cpp` | Testbench: 10 back-to-back runs of random int8 data plus corner values, with tag check |
| `hls_config.cfg` | HLS configuration (`clock=4ns`) |
| `run.py` | Vitis flow: C simulation, synthesis, co-simulation |
| `description.json` | Example metadata |

## Running

```
vitis -s run.py
```

Run under Vitis 2026.2. The flow creates `work_double_pump_using_blackbox/` in
this directory.

## Results

Vitis 2026.2, `clock=4ns`:

- C simulation: 0 errors
- C synthesis: completed successfully
- C/RTL co-simulation: PASS

## Timing

- Base clock: 4 ns. The 2x clock is 2 ns.
- Latency: 5 base-clock cycles, set in `bb/dp_mac4.json` with input
  `take_effect_stage` 1 and output `take_effect_stage` 5.
- The 1x registers honour `ap_ce`. The 2x section runs free-running, and `dp_top`
  issues one call per cycle.

## Hardware notes

- Generate both clocks from one MMCM, with the 2x output exactly twice the base
  clock, and connect the 2x output to the IP pin `ap_clk_mult1_div2`.
- Match the clock trees of the two clock nets (for example, with `CLOCK_DELAY_GROUP`).
- Multicycle paths on the datapath registers, because `opa`/`opb` capture on every 2x edge:
  - `xr`/`wr` to `opa`/`opb` (taken on even edges): setup multicycle 2 (`-end`), hold 1
  - `xd`/`wd` to `opa`/`opb` (taken on the odd edge): no multicycle constraint
  - `outq` to `res` (loads only on even edges): setup multicycle 2 (`-start`), hold 1
- After synthesis, `dp_mac4` should contain four DSP48E2 blocks, all on the 2x clock.
