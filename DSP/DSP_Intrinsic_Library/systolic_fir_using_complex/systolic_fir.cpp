#include "systolic_fir.h"

// example showing cascades to infer "size" DSPCPLX and using cascaded connections
template <int size>
class FIR{
     private:
         const A_t* coeff_;
         C_t bias_;
         cascade<BALANCED_PIPELINE|REG_M|REG_P > dsp0;
         cascade<    FULL_PIPELINE|REG_M|REG_P > dsp_full[size - 1]; 
     public:
        FIR(const A_t* coeff, C_t bias) : coeff_(coeff), bias_(bias) {
            #pragma HLS ARRAY_PARTITION variable=dsp_full complete dim=1
        };

        C_t fir(B_t input){
            auto out = dsp0.mul_add(coeff_[0], input, bias_);
            for(int j=1; j < size ; j++){
                #pragma HLS unroll
                out = dsp_full[j - 1].mul_add(coeff_[j], out.bcout, out.pcout);
            }
            return out.pcout;
        }; // end of fir function
}; // end of FIR class

void systolic_fir( A_t coeff[TAPS], B_t b[NDATA], C_t bias, C_t hw[NDATA]) {
    #pragma HLS ARRAY_PARTITION variable=coeff complete dim=1
   
    FIR<TAPS> my_fir(coeff, bias);

    LOOP_FIR:
    for(auto i = 0 ; i< NDATA ; i++){
        #pragma HLS pipeline II=1
        hw[i] = my_fir.fir(b[i]);
    }
}

