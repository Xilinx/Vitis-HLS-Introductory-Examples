#include "systolic_fir.h"
#include <iostream>

int main(void) {
    ap_int<18> coeff_R[TAPS] = {1, 12, 3, 4};
    A_t coeff[TAPS];
    ap_int<18> b_R[NDATA] = {2, 0, 0, 0, 3, -2, -7, 200, 120, 64};
    B_t b[NDATA];
    A_t bias = {0, 0};
    C_t sw[NDATA];
    C_t hw[NDATA];

    for (int i = 0; i < TAPS; i++) {
        coeff[i] = {coeff_R[i],i};
    }
    for (int i = 0; i < NDATA; i++) {
        b[i] = {b_R[i],i};
    }

    // Software implementation
    // This is a simple software implementation of the FIR filter
    // It is only used for testing and verification purposes
    class systolic_fir_sw{
        public:
            systolic_fir_sw(const A_t* coeff, int size, A_t bias)
                : coeff_(coeff), size_(size), bias_(bias) {
                    shift_line_ = new A_t[size];
                    for( int i = 0; i< size; ++i){
                        shift_line_[i] = 0;
                    }
                }
            ~systolic_fir_sw() {
                delete[] shift_line_;
            }
            C_t fir(A_t input) {
                for(int i = TAPS-1 ; i > 0 ; --i ){
                    shift_line_[i] = shift_line_[i - 1];
                }

                shift_line_[0] = input;

                C_t output = bias_;
                for (int i = 0; i < size_; i++ ){
                    output += coeff_[i] * shift_line_[i];
                }
                return output;
            }
        private:
            const B_t* coeff_;
            A_t* shift_line_;
            int size_;
            A_t bias_;
    };

    systolic_fir_sw fir_filter(coeff, TAPS, bias);
    for (int i = 0 ; i < NDATA; i ++){
        sw[i] = fir_filter.fir(b[i]);
    }

    systolic_fir(coeff,b, bias, hw); // calling DUT
    int errors = 0;
    for(int j = 0; j < NDATA; j++){
        // std::cout << "Input " << b[j] << ", Expect " << sw[j] << ", Got " << hw[j] << std::endl;
        std::cout << "Input {" << b[j].real() << "," << b[j].imag() << "}, "\
            <<"Expect {" << sw[j].real() << "," << sw[j].imag() << "}, "\
            << "Got {" << hw[j].real() << "," << hw[j].imag() << "}" ;
        // if (hw[j] != sw[j]) {
        if (hw[j].real() != sw[j].real() || hw[j].imag() != sw[j].imag()) {
            errors++;
            std::cout << "<-- Error" << std::endl;
        } else {
            std::cout << "<-- OK" << std::endl;
        }
    }
    if (errors > 0) {
        std::cout << "FAILED" << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "PASS" << std::endl;
        return EXIT_SUCCESS;
    }
}

