/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
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

#ifndef DDS_H_
#define DDS_H_

#include <fstream>
#include <iostream>
using namespace std;

#include "ap_fixed.h"
#include <math.h>

// phase accumulator
typedef ap_fixed<32, 1> incr_t; // s.xxxx, +/- 1 = +/- pi = +fs/2 to -fs/2
typedef ap_fixed<32, 1> acc_t;  // s.xxxx, +/- 1 = +/- pi = +fs/2 to -fs/2

// cos lut address, word size
const int NLUT = 10;      // bitwidth for cos lut address, covers one quadrant
const int LUTSIZE = 1024; // 2^NLUT
typedef ap_uint<NLUT + 2> lut_adr_t; // covers 4 quadrant
typedef ap_uint<NLUT> quad_adr_t;    // covers 1 quadrant

// rounding makes huge difference in the noise floor
typedef ap_fixed<18, 1, AP_RND_CONV, AP_SAT_SYM> lut_word_t;
// typedef ap_fixed<18,1> lut_word_t;
// typedef double lut_word_t;

// fine lut address, word size
const int NFINE = 9;      // bitwidth for fine lut address, covers one quadrant
const int FINESIZE = 512; // 2^NFINE
typedef ap_uint<NLUT + 2> fine_adr_t; // covers 4 quadrant
// typedef ap_fixed<18,1,AP_RND_INF,AP_SAT_SYM> fine_word_t;
// typedef ap_fixed<18,1> fine_word_t;
// typedef ap_fixed<18,-8> fine_word_t;
// typedef ap_fixed<18,-10> fine_word_t;
// typedef double fine_word_t;
typedef ap_fixed<18, -7> fine_word_t;

const double DELTA =
    M_PI / (2 * LUTSIZE *
            FINESIZE); // fine lut resolution, range covers 0 to pi/(2*LUTSIZE)

// DDS output = f(cos lut, fine table)
// typedef ap_fixed<18,1,AP_RND_CONV,AP_SAT_SYM> dds_t;
typedef ap_fixed<16, 1, AP_RND_CONV, AP_SAT_SYM> dds_t;

//______________________________________________________________________________
// N:        number of sample to process
// DDS_T:    cos/sin data type
// OUTPUT_T: output data type
//______________________________________________________________________________

template <int N, typename DDS_T, typename OUTPUT_T> class dds_class {

    // tables
    lut_word_t cos_lut[LUTSIZE];
    fine_word_t fine_lut[FINESIZE];

    // accumulator
    acc_t acc;

    // for dithering
    // ap_uint<1> sr[19];

  public:
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    dds_class() {
        acc = 0;
        // sr[19] = {0,0,0,0,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1};

        // initialize tables
        init_cos_lut(cos_lut, LUTSIZE);
        init_fine_lut(fine_lut, FINESIZE, DELTA);
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void mix(DDS_T* dds_cos, DDS_T* dds_sin, OUTPUT_T* dout_i, OUTPUT_T* dout_q,
             OUTPUT_T* dout) {

        // static OUTPUT_T  dout_tmp;
        OUTPUT_T dout_tmp;

    L1:
        for (int i = 0; i < N; i++) {
#pragma HLS pipeline rewind
            dout[i] = dout_tmp;
            dout_tmp = dout_i[i] * dds_cos[i] - dout_q[i] * dds_sin[i];
        }
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void init_cos_lut(lut_word_t cos_lut[LUTSIZE], const int LUTSIZE) {

        double cos_double;
// ofstream fp_dout ("debug.txt");

// #define FULL
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
#ifdef MIDPOINT

        // store single quadrant
        for (int i = 0; i < LUTSIZE; i++) {
            // cos_double = cos(2*M_PI*(0.0+(double)i)/(4*LUTSIZE));
            cos_double = cos(2 * M_PI * (0.5 + (double)i) / (4 * LUTSIZE));
            cos_lut[i] = cos_double;
            //      fp_dout << scientific << cos_double <<endl;
        }

#ifdef FULL
        // store full quadrant
        ofstream fp_ideal("ideal.txt");
        for (int i = 0; i < 4 * LUTSIZE; i++) {
            cos_double = cos(2 * M_PI * (0.5 + (double)i) / (4 * LUTSIZE));
            fp_ideal << scientific << cos_double << endl;
        }
#endif

#else
        //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
        // not the mid point

        // store single quadrant
        for (int i = 0; i < LUTSIZE; i++) {
            cos_double = cos(2 * M_PI * (0.0 + (double)i) / (4 * LUTSIZE));
            cos_lut[i] = cos_double;
            // fp_dout << scientific << cos_double <<endl;
        }

#ifdef FULL
        // store full quadrant
        ofstream fp_ideal("ideal.txt");
        for (int i = 0; i < 4 * LUTSIZE; i++) {
            cos_double = cos(2 * M_PI * (0.0 + (double)i) / (4 * LUTSIZE));
            fp_ideal << scientific << cos_double << endl;
        }
#endif

#endif
    }

#if 0
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void read_cos_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE ) {

  lut_adr_t i;            // cover full quadrant
  quad_adr_t lsb,adr;     // cover 1/4 quadrant
  ap_uint<2>  msb;        // specify which quadrant
  lut_word_t  lut_word; 

//  ofstream fp_dout ("fullcos.txt");

  for (int k=0;k<4*LUTSIZE;k++) {

    i    = k;
    msb  = i(11,10);
    lsb  = i(9,0);

    if (msb==0) {        // right top
       adr      = lsb;
       lut_word = cos_lut[adr];
    } else if (msb==1) {  // left top 
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word = -cos_lut[adr];
       }
    } else if (msb==3) {  // right bot
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word =  cos_lut[adr];
       }
    } else             {  // left bot 
         adr      =  lsb;
         lut_word = -cos_lut[adr];
    }

//    fp_dout << scientific << lut_word << endl;

  }

}

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void read_sine_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE ) {

  lut_adr_t i;            // cover full quadrant
  quad_adr_t lsb,adr;     // cover 1/4 quadrant
  ap_uint<2>  msb;        // specify which quadrant
  lut_word_t  lut_word; 

//  ofstream fp_dout ("fullsine.txt");

  for (int k=0;k<4*LUTSIZE;k++) {

    i    = k;
    msb  = i(11,10);
    lsb  = i(9,0);

    if (msb==1) {         // left top
       adr      = lsb;
       lut_word = cos_lut[adr];
    } else if (msb==2) {  // left bot 
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word = -cos_lut[adr];
       }
    } else if (msb==0) {  // right top
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word =  cos_lut[adr];
       }
    } else             {  // right bot 
         adr      =  lsb;
         lut_word = -cos_lut[adr];
    }

//    fp_dout << scientific << lut_word << endl;

  }

}
#endif

#if 0
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
//dithering
// z^19 + z^18 + z^14 + 1, from mathworks
// x^19 + x^6  + x^5 + x + 1, used here, not sure if it is optimum
//
ap_uint<19> dither() {

ap_uint<1> sin;
 
sin = sr[18] ^ sr[5] ^ sr[4] ^ sr[0];

LOOP_sr: 
for (int i=18; i>0; i--) {
    sr[i] = sr[i-1];
}

sr[0] = sin;

//for (int j=18; j>=0; j--)
    //cout<< setw(2) << sr[j];
//cout << endl;

ap_uint<19> whole;
LOOP_whole: 
    for (int i=0; i<19; i++) whole[i] = sr[i];
    //cout << ":   whole = " << setw(16) << whole << endl;
    //cout << setw(16) << whole << endl;

return whole;

}

#endif

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void init_fine_lut(fine_word_t fine_lut[FINESIZE], const int FINESIZE,
                       const double delta) {

        // double fine_double;
        double sine_double;
        // ofstream fp_dout ("fine.txt");

        for (int i = 0; i < FINESIZE; i++) {
            // fine_double = cos(delta*(double)i);
            sine_double = sin(delta * (double)i);
            fine_lut[i] = sine_double;
            // fp_dout << scientific << fine_double <<", " << scientific <<
            // sine_double <<endl;
        }
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void process(incr_t incr, DDS_T* cos_out, DDS_T* sin_out) {

        fine_adr_t fine_adr;
        fine_word_t fine_word;

        lut_adr_t full_adr; // cover full quadrant
        quad_adr_t lsb;     // cover 1/4 quadrant
        quad_adr_t cos_adr, sin_adr;

        ap_uint<2> msb; // specify which quadrant
        lut_word_t cos_lut_word;
        lut_word_t sin_lut_word;

        // phase accumulator
        acc += incr;

        // look up cos/sine table
        full_adr = acc(31, 20);
        msb = full_adr(11, 10);
        lsb = full_adr(9, 0);

        // right top
        if (msb == 0) {
            cos_adr = lsb;
            cos_lut_word = cos_lut[cos_adr];

            if (lsb == 0)
                sin_lut_word = 0;
            else {
                sin_adr = -lsb;
                sin_lut_word = cos_lut[sin_adr];
            }

            // left top
        } else if (msb == 1) {
            if (lsb == 0)
                cos_lut_word = 0;
            else {
                cos_adr = -lsb;
                cos_lut_word = -cos_lut[cos_adr];
            }

            sin_adr = lsb;
            sin_lut_word = cos_lut[sin_adr];

            // right bot
        } else if (msb == 3) {
            if (lsb == 0)
                cos_lut_word = 0;
            else {
                cos_adr = -lsb;
                cos_lut_word = cos_lut[cos_adr];
            }
            sin_adr = lsb;
            sin_lut_word = -cos_lut[sin_adr];

            // left bot
        } else {
            cos_adr = lsb;
            cos_lut_word = -cos_lut[cos_adr];

            if (lsb == 0)
                sin_lut_word = 0;
            else {
                sin_adr = -lsb;
                sin_lut_word = -cos_lut[sin_adr];
            }
        }

        // adjustment w/ fine table
        fine_adr = acc(19, 11);
        fine_word = fine_lut[fine_adr];

        DDS_T cos_dds, sin_dds;
        cos_dds = cos_lut_word - sin_lut_word * fine_word;
        sin_dds = sin_lut_word + cos_lut_word * fine_word;

        *cos_out = cos_dds;
        *sin_out = sin_dds;
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void process_frame(incr_t incr, DDS_T* cos_out, DDS_T* sin_out) {

        DDS_T dds_cos;
        DDS_T dds_sin;

    L1:
        for (int i = 0; i < N; i++) {
#pragma HLS pipeline rewind
            process(incr, &dds_cos, &dds_sin);
            cos_out[i] = dds_cos;
            sin_out[i] = dds_sin;
        }
    }

}; // dds_class

#endif
