/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// ============================================================================
// dp_mac4 -- double-pumped 4-lane int8 MAC.  ONE multiplier per lane, clocked
// at 2x, does TWO products per base cycle:
//     lane l result = w[2l]*x[2l]  (2x phase 0)  +  w[2l+1]*x[2l+1]  (phase 1)
// so 4 multipliers deliver 8 MAC per base cycle.  Same C contract as
// bb/dp_mac4.cpp (pure function, no state).
//
//   In = {tag[7:0], Wpack[63:0], Xpack[63:0]}   int8 element i at bits i*8 +: 8
//   ap_return = {tag[7:0], lanes[127:0]}        lane l = bits l*32 +: 32 (int32)
//   The TAG is delayed 5 base cycles (same as the result) so it comes out beside
//   the result of the call it went in with.
//
// CLOCKS: ap_clk (base) and ap_clk_div2 (the 2x clock -- HLS names it after
// derived_period_div=2).  Edges of the 2x clock that coincide with an ap_clk
// edge are called EVEN, the ones in between ODD.
//
// PHASE RECOVERY : a 1x toggle FLOP t1
// is sampled in the 2x domain; s ^ s_d is 1 exactly when the NEXT 2x edge is
// even.  No clock-net-as-data, no negedge flop.
//
// TIMELINE (B0 = the base edge that samples In; E = B1 is the next even edge)
//   B0   xr/wr <= In                       (1x)
//   B1   wd/xd <= xr/wr                    (1x)  pair-1 operands wait one base
//        2x: opa/opb <= pair 0 (from wr/xr)         cycle so BOTH pairs belong
//   B1+h 2x: prod <= p0 ; opa/opb <= pair 1 (wd/xd) to the same In sample
//   B2   2x: ph0 <= p0 ; prod <= p1
//   B2+h 2x: sum <= p0 + p1                (odd edge)
//   B3   2x: outq <= sum                   (even edge: the 2x->1x hop is a
//                                           full base period)
//   B4   1x: res <= outq                   -> ap_return
// Latency is 5 base cycles: input take_effect_stage 1, output stage 5, latency 5 in bb/dp_mac4.json.
// The 1x registers honour ap_ce; the 2x section runs free.  dp_top keeps the box fed every cycle.
// ============================================================================
`timescale 1 ns / 1 ps

module dp_mac4 #(
    parameter USE_CE = 1
) (
    input              ap_clk,
    input              ap_clk_div2,     // the 2x clock (HLS naming)
    input              ap_rst,
    input              ap_ce,
    input      [135:0] In,
    output     [135:0] ap_return
);

wire ce1 = USE_CE ? ap_ce : 1'b1;
wire [63:0] Xin = In[63:0];
wire [63:0] Win = In[127:64];

// ---- phase recovery -------------------------------------------------------
reg t1 = 1'b0;
always @(posedge ap_clk) t1 <= ~t1;
reg s = 1'b0, s_d = 1'b0;
always @(posedge ap_clk_div2) begin s <= t1; s_d <= s; end
wire nE = s ^ s_d;                         // 1 <=> the next 2x edge is even

// ---- 1x side --------------------------------------------------------------
reg [63:0] xr = 64'd0, wr = 64'd0, xd = 64'd0, wd = 64'd0;
always @(posedge ap_clk)
    if (ce1) begin
        xr <= Xin;  wr <= Win;
        xd <= xr;   wd <= wr;
    end

// ---- 2x side: one multiplier per lane -------------------------------------
reg signed [7:0]  opa  [0:3];
reg signed [7:0]  opb  [0:3];
(* use_dsp = "yes" *) reg signed [15:0] prod [0:3];
reg signed [15:0] ph0  [0:3];
reg signed [16:0] sum  [0:3];
reg signed [16:0] outq [0:3];
integer l;
initial for (l = 0; l < 4; l = l + 1) begin
    opa[l] = 0; opb[l] = 0; prod[l] = 0; ph0[l] = 0; sum[l] = 0; outq[l] = 0;
end
always @(posedge ap_clk_div2)
    for (l = 0; l < 4; l = l + 1) begin
        // even edge picks pair 0 (element 2l), odd edge pair 1 (element 2l+1)
        opa[l]  <= nE ? $signed(wr[(2*l)*8   +: 8]) : $signed(wd[(2*l+1)*8 +: 8]);
        opb[l]  <= nE ? $signed(xr[(2*l)*8   +: 8]) : $signed(xd[(2*l+1)*8 +: 8]);
        prod[l] <= opa[l] * opb[l];
        if (nE)  ph0[l]  <= prod[l];                  // even edge: prod = p0
        if (!nE) sum[l]  <= ph0[l] + prod[l];         // odd edge : ph0 = p0, prod = p1
        if (nE)  outq[l] <= sum[l];                   // even edge: stable for a base period
    end

// ---- tag: same 5 base-cycle delay as the data (B0 .. B4) -----------------------
reg [7:0] tg0 = 8'd0, tg1 = 8'd0, tg2 = 8'd0, tg3 = 8'd0, tg4 = 8'd0;
always @(posedge ap_clk)
    if (ce1) begin
        tg0 <= In[135:128];
        tg1 <= tg0;  tg2 <= tg1;  tg3 <= tg2;  tg4 <= tg3;
    end
assign ap_return[135:128] = tg4;

// ---- back to 1x -------------------------------------------------------------
reg signed [16:0] res [0:3];
integer k;
initial for (k = 0; k < 4; k = k + 1) res[k] = 0;
always @(posedge ap_clk)
    if (ce1) for (k = 0; k < 4; k = k + 1) res[k] <= outq[k];

genvar g;
generate for (g = 0; g < 4; g = g + 1) begin : GO
    assign ap_return[g*32 +: 32] = {{15{res[g][16]}}, res[g]};
end endgenerate

endmodule
