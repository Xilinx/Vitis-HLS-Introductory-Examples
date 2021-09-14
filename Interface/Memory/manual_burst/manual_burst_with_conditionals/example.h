#include "hls_burst_maxi.h"

#ifndef _LOOP_SEQUENTIAL_H_
#define _LOOP_SEQUENTIAL_H_

#include "ap_int.h"
#define N 800

typedef ap_int<30> din_t;
typedef ap_int<20> dout_t;

typedef struct {
	din_t A;
	dout_t B;
}  DUO;

void example(hls::burst_maxi<din_t> A, hls::burst_maxi<din_t> B, hls::burst_maxi<dout_t> RES, din_t factor);

#endif

