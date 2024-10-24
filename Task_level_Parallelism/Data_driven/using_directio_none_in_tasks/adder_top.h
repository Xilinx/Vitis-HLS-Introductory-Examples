#ifndef __ADDER_TOP_H__
#define __ADDER_TOP_H__
#include "hls_task.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "hls_directio.h"
const int L_INPUT = 1;
const int L_OUTPUT = 1;
static const int N_INT = 8;
static const int N_FRAC = 0;

typedef hls::stream<int> buf;

typedef hls::ap_none<int> mystream_in;
typedef hls::ap_none<int> mystream_out;
typedef hls::ap_none<int> stream_vld_in;
typedef hls::ap_none<int> stream_vld_out;


void adder_top(mystream_in &din_A, mystream_in &din_B, mystream_out &dout,stream_vld_in &vldIn, stream_vld_out & vldOut);

#endif
