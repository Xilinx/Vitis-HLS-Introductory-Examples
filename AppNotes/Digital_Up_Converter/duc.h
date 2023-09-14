#ifndef DUC_H
#define DUC_H

#include <fstream>
#include <iostream>
using namespace std;

const int L_INPUT  = 200;
const int L_OUTPUT = 16*L_INPUT;

#include "dds.h"
#include "fir.h"


//______________________________________________________________________________
// filter spec
// - putting these inside the duc class makes it not compile

// SRRC
static const int Lsrrc_WHOLE = 65;
const COEF_T cin_srrc[Lsrrc_WHOLE] = {
//      #include "srrc_0db.inc"
8.538552e-06, 4.669178e-07, -3.697474e-05, 3.052152e-06, 1.093230e-04, -2.416268e-05, -2.637924e-04, 9.271878e-05, 5.516021e-04, -2.673026e-04, -1.035981e-03, 6.447576e-04, 1.781800e-03, -1.374126e-03, -2.845114e-03, 2.666081e-03, 4.255749e-03, -4.808402e-03, -6.004074e-03, 8.189170e-03, 8.028184e-03, -1.336089e-02, -1.021252e-02, 2.121003e-02, 1.239342e-02, -3.346770e-02, -1.437864e-02, 5.450117e-02, 1.597204e-02, -1.003560e-01, -1.700502e-02, 3.163505e-01, 5.173627e-01, 3.163505e-01, -1.700502e-02, -1.003560e-01, 1.597204e-02, 5.450117e-02, -1.437864e-02, -3.346770e-02, 1.239342e-02, 2.121003e-02, -1.021252e-02, -1.336089e-02, 8.028184e-03, 8.189170e-03, -6.004074e-03, -4.808402e-03, 4.255749e-03, 2.666081e-03, -2.845114e-03, -1.374126e-03, 1.781800e-03, 6.447576e-04, -1.035981e-03, -2.673026e-04, 5.516021e-04, 9.271878e-05, -2.637924e-04, -2.416268e-05, 1.093230e-04, 3.052152e-06, -3.697474e-05, 4.669178e-07, 8.538552e-06 
};

// HB1
static const int Lhb1_WHOLE   = 23;
const COEF_T cin_hb1[Lhb1_WHOLE] = {
//      #include "b1_fp2.inc"
-4.347138e-04, 0.000000e+00, 2.889073e-03, 0.000000e+00, -1.100753e-02, 0.000000e+00, 3.178616e-02, 0.000000e+00, -8.312888e-02, 0.000000e+00, 3.098590e-01, 4.999390e-01, 3.098590e-01, 0.000000e+00, -8.312888e-02, 0.000000e+00, 3.178616e-02, 0.000000e+00, -1.100753e-02, 0.000000e+00, 2.889073e-03, 0.000000e+00, -4.347138e-04 
};

// HB2
static const int Lhb2_WHOLE   = 11;
const COEF_T cin_hb2[Lhb2_WHOLE] = {
//      #include "b2_fp2.inc"
7.416439e-03, 0.000000e+00, -5.306043e-02, 0.000000e+00, 2.956309e-01, 4.999390e-01, 2.956309e-01, 0.000000e+00, -5.306043e-02, 0.000000e+00, 7.416439e-03 
};

// HB3
static const int Lhb3_WHOLE   = 7;
const COEF_T cin_hb3[Lhb3_WHOLE] = {
//      #include "b3_fp2.inc"
-3.216511e-02, 0.000000e+00, 2.821170e-01, 4.999390e-01, 2.821170e-01, 0.000000e+00, -3.216511e-02 
};
  
static const int II_SRRC = 16;
static const int II_HB1  = 8;
static const int II_HB2  = 4;
static const int II_HB3  = 2;

//______________________________________________________________________________
void duc ( DATA_T din_i[L_INPUT], DATA_T din_q[L_INPUT], DATA_T dout[L_OUTPUT], 
	       incr_t incr );


//______________________________________________________________________________
// multi stage filter
// - 4 stages, SRRC -> HB1-> HB2-> HB3 -> to DAC
//______________________________________________________________________________

template<int l_INPUT>
class filterStageClass {

public:

#if 0
// constructor not compiling
interp2_class<Lsrrc_WHOLE,   l_INPUT,   II_SRRC> srrc(cin_srrc);
interp2_hb_class<Lhb1_WHOLE, 2*l_INPUT, II_HB1>  hb1(cin_hb1);
interp2_hb_class<Lhb2_WHOLE, 4*l_INPUT, II_HB2>  hb2(cin_hb2);
interp2_hb_class<Lhb3_WHOLE, 8*l_INPUT, II_HB3>  hb3(cin_hb3);
#endif

interp2_class<Lsrrc_WHOLE,   l_INPUT,   II_SRRC> srrc;
interp2_hb_class<Lhb1_WHOLE, 2*l_INPUT, II_HB1>  hb1;
interp2_hb_class<Lhb2_WHOLE, 4*l_INPUT, II_HB2>  hb2;
interp2_hb_class<Lhb3_WHOLE, 8*l_INPUT, II_HB3>  hb3;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process(DATA_T din[l_INPUT], DATA_T dout[16*l_INPUT] ) {
#pragma HLS dataflow

    DATA_T srrc_dout[2*l_INPUT];
    DATA_T hb1_dout[4*l_INPUT];
    DATA_T hb2_dout[8*l_INPUT];
    srrc.process_frame(din, srrc_dout);
    hb1.process_frame(srrc_dout, hb1_dout);
    hb2.process_frame(hb1_dout, hb2_dout);
    hb3.process_frame(hb2_dout, dout);
}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
#if 1
//constructor
filterStageClass( void ) { 
        srrc.init(cin_srrc);
        hb1.init(cin_hb1);
        hb2.init(cin_hb2);
        hb3.init(cin_hb3);
}

//destructor
~filterStageClass( void ) { }
#endif

};


//______________________________________________________________________________
// 2 channel multi-stage filter
//______________________________________________________________________________
//template<int l_INPUT>
template<int l_INPUT, const int l_OUTPUT = 16*l_INPUT>
class filterStageClassTwoChan {

filterStageClass<l_INPUT> duc_i;
filterStageClass<l_INPUT> duc_q;

public:

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process(DATA_T din_i[l_INPUT], DATA_T dout_i[l_OUTPUT],
             DATA_T din_q[l_INPUT], DATA_T dout_q[l_OUTPUT]) {
#pragma HLS INLINE
    duc_i.process(din_i, dout_i);
    duc_q.process(din_q, dout_q);
}

}; 



#endif
