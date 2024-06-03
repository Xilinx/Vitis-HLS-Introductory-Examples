/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
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

#ifndef _FIR_H
#define _FIR_H

//#include <fstream>
#include <iostream>
// using namespace std;

//______________________________________________________________________________
// filter precision
// - precisions used by all the FIR class
//______________________________________________________________________________

#define FIXED

#ifdef FIXED
#include <ap_fixed.h>

static const int NFRAC = 14;
typedef ap_fixed<3 + NFRAC, 3, AP_TRN, AP_WRAP> DATA_T;
typedef ap_fixed<4 + NFRAC, 4, AP_TRN, AP_WRAP> DATA2_T;
typedef ap_fixed<2 + NFRAC, 2, AP_RND_CONV, AP_WRAP> COEF_T;
typedef ap_fixed<5 + NFRAC, 5, AP_RND_CONV, AP_WRAP>
    PROD_T; // rounding helps ~20db dc offset
typedef ap_fixed<10 + NFRAC, 10, AP_TRN, AP_WRAP> ACC_T;
#else
typedef float DATA_T;
typedef float DATA2_T;
typedef float COEF_T;
typedef float PROD_T;
typedef float ACC_T;
#endif

//______________________________________________________________________________
// interpolate by 2 FIR class
//
// - make sure the coeff is odd length, so both subfilter will be symmetric
// - parameter:
//    -l_WHOLE:   number of taps of prototype filter (not the decomposed
//    subfilter) -l_INPUT:  number of data samples -II_GOAL:   initiation
//    interval goal
//
//______________________________________________________________________________

template <int l_WHOLE, int l_INPUT, int II_GOAL> class interp2_class {

    // use assert to check the template parameter

    static const int INTERP_FACTOR = 2;
    static const int L_SUB = l_WHOLE / INTERP_FACTOR; // 32
    static const int ODD = l_WHOLE % 2;               // 1
    static const int l_TDL = L_SUB + ODD;             // 33
    static const int l_NONZERO = L_SUB / 2 + ODD;     // 17

    DATA_T sr[l_TDL];
    ACC_T acc0, acc1;
    COEF_T coeff[2][l_NONZERO];

  public:
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    // MAC engine
    ACC_T MAC_preadd(DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc) {

        DATA2_T preadd = din0 + din1;
        PROD_T prod = preadd * coef;
        ACC_T sum = prod + acc;

        return sum;
    };

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void process(DATA_T din, DATA_T* dout0, DATA_T* dout1) {

#pragma HLS INLINE

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable = sr complete
#pragma HLS array_reshape variable = coeff complete dim = 0

        acc0 = 0;
        acc1 = 0;

    LOOP_MAC:
        for (int i = 0; i < l_NONZERO - ODD; i++) {

            // even number of taps, has one more than odd one
            acc0 = MAC_preadd(sr[i], sr[l_TDL - 1 - i], coeff[0][i], acc0);

            // odd number of taps
            acc1 =
                MAC_preadd(sr[i], sr[l_TDL - 1 - ODD - i], coeff[1][i], acc1);
        }

        // center tap
        acc0 = MAC_preadd(sr[l_NONZERO - 1], 0, coeff[0][l_NONZERO - 1], acc0);

    LOOP_SR:
        for (int i = l_TDL - 1; i > 0; i--) {
            sr[i] = sr[i - 1];
        }

        sr[0] = din;

        *dout0 = acc0;
        *dout1 = acc1;
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    void process_frame(DATA_T din[l_INPUT], DATA_T dout[2 * l_INPUT]) {

        DATA_T dout0, dout1;
    L_process_frame:
        for (int i = 0; i < l_INPUT; i++) {
#pragma HLS pipeline II = II_GOAL rewind
            process(din[i], &dout0, &dout1);
            dout[2 * i] = dout0;
            dout[2 * i + 1] = dout1;
        }
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    // initialize coefficient for polyphase decomposition
    // - test out even length
    void init(const COEF_T coef_in[l_WHOLE]) {

        static const int nofphase = 2;

        bool scale = 1;
        COEF_T gain = scale ? 2 : 1; // only for DUC

        for (int i = 0; i < l_NONZERO - ODD; i++)
            for (int k = 0; k < 2; k++) {

                coeff[k][i] = gain * coef_in[2 * i + k];
                // cout <<"["<<k<<"]"<<"["<<i<<"] = " <<2*i+k <<endl;
            }

        // number of taps is one greater for odd phase filter
        if (ODD) {

            int k = 0;
            int i = l_NONZERO - 1;

            coeff[k][i] = gain * coef_in[2 * i];
            // cout <<"["<<k<<"]"<<"["<<i<<"] = " <<2*i <<"<=== last " << endl;
        }
    }

    // constructor
    interp2_class(const COEF_T cin[l_WHOLE]) { init(cin); }

    interp2_class(void) {}

    // destructor
    ~interp2_class(void) {}

}; // interp2_class

//______________________________________________________________________________
// interpolate by 2 halfband fir class
//
// - parameter:
//    -l_WHOLE:   number of taps
//    -l_INPUT:  number of input data samples
//    -II_GOAL:   initiation interval goal
//
//
//    -l_TDL:     number of stages in tap delay line
//    -l_NONZERO: number of unique coefficients
//______________________________________________________________________________

template <int l_WHOLE, int l_INPUT, int II_GOAL> class interp2_hb_class {

    // use assert to check the template parameter
    //  (l_WHOLE+1) % 4 == 0?

    static const int l_TDL = (l_WHOLE + 1) / 2; // (23+1)/2 = 12
    static const int l_NONZERO = 1 + l_TDL / 2; // 1 + 12/2 = 7

    DATA_T sr[l_TDL];
    ACC_T acc;
    COEF_T coeff[l_NONZERO];

  public:
    //_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
    //_
    //_
    // MAC engine
    ACC_T MAC_preadd(DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc) {

        DATA2_T preadd = din0 + din1;
        PROD_T prod = preadd * coef;
        ACC_T sum = prod + acc;

        return sum;
    };

    //_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
    //_
    //_
    // filter
    void process(DATA_T din, DATA_T* dout0, DATA_T* dout1) {

#pragma HLS array_reshape variable = sr complete
#pragma HLS array_reshape variable = coeff complete dim = 0

        acc = 0;
    LOOP_MAC:
        for (int i = 0; i < l_NONZERO - 1; i++) {
            acc = MAC_preadd(sr[i], sr[l_TDL - 1 - i], coeff[i], acc);
        }

    LOOP_SR:
        for (int i = l_TDL - 1; i > 0; i--) {
            sr[i] = sr[i - 1];
        }

        sr[0] = din;

        *dout0 = acc;
        *dout1 = coeff[l_NONZERO - 1] * sr[l_NONZERO - 1];
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _
    // filter frame
    void process_frame(DATA_T din[l_INPUT], DATA_T dout[2 * l_INPUT]) {

        DATA_T dout0, dout1;

    L_process_frame:
        for (int i = 0; i < l_INPUT; i++) {
#pragma HLS pipeline II = II_GOAL rewind
            process(din[i], &dout0, &dout1);
            dout[2 * i] = dout0;
            dout[2 * i + 1] = dout1;
        }
    }

    //_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
    //_
    //_
    // initialize coefficient for halfband filter
    void init(const COEF_T cin[]) {

        bool scale = 1;
        COEF_T gain = scale ? 2 : 1;

        for (int i = 0; i < l_NONZERO - 1; i++) {
            coeff[i] = gain * cin[2 * i];
        }

        int i = l_NONZERO - 1;
        coeff[i] = gain * cin[2 * i - 1];
    };

    // constructor
    interp2_hb_class(const COEF_T cin[l_WHOLE]) { init(cin); }

    interp2_hb_class(void) {}

    // destructor
    ~interp2_hb_class(void) {}

}; // interp2_hb_class

#endif
