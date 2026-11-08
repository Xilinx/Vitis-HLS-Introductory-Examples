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
#include <cstdio>
#include <cstdlib>
#include "dp_top.h"

// RANDOM TEST, NRUN back-to-back dp_top calls (= NRUN cosim transactions).
// Every word of every run is fresh random int8 data, so no two neighbouring
// words are equal (identical words would hide a duplicated/dropped result).
// Extreme corners are planted mid-stream (words 100..103).  The tag outputs are checked too.
static const int NRUN = 10;

int main() {
    static ap_uint<64> x[NW], w[NW];
    int total_bad = 0;
    srand(1234);
    for (int run = 0; run < NRUN; run++) {
        long long gold[4] = {0, 0, 0, 0};
        for (int i = 0; i < NW; i++) {
            for (int e = 0; e < 8; e++) {
                int xv = (rand() % 256) - 128;
                int wv = (rand() % 256) - 128;
                if (i == 100) { xv = -128; wv = -128; }   // +16384 per product
                if (i == 101) { xv = -128; wv = 127;  }   // extreme negative
                if (i == 102) { xv = 127;  wv = 127;  }   // extreme positive
                if (i == 103) { xv = 127;  wv = -128; }
                x[i].range(e * 8 + 7, e * 8) = (ap_uint<8>)(ap_int<8>)xv;
                w[i].range(e * 8 + 7, e * 8) = (ap_uint<8>)(ap_int<8>)wv;
                gold[e / 2] += (long long)xv * wv;
            }
        }
        ap_int<32> out[6];
        dp_top(x, w, out);
        int bad = 0;
        for (int l = 0; l < 4; l++) {
            printf("run %d lane %d: got %d  gold %lld\n", run, l, (int)out[l], gold[l]);
            if ((long long)out[l] != gold[l]) bad++;
        }
        printf("run %d tag check: n_bad = %d  first_bad = %d\n", run, (int)out[4], (int)out[5]);
        if ((int)out[4] != 0) bad++;
        total_bad += bad;
    }
    printf("RESULT: %s (%d runs, %d mismatches)\n", total_bad ? "FAIL" : "PASS", NRUN, total_bad);
    return total_bad ? 1 : 0;
}
