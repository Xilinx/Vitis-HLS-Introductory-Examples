#include "systolic_fir.h"
#include <iostream>

int main(void) {
    long a[TAPS] = {1, 12, 3, 4};
    long b[N] = {2, 0, 0, 0, 3, -2, -7, 200, 12000, 64};
    long c = 0;
    long sw[N];
    long hw[N];

    class systolic_fir_sw{
        public:
            systolic_fir_sw(const long* coeff, int size)
                : coeff_(coeff), size_(size) {
                    shift_line_ = new long[size_];
                    for( int i = 0; i< size; ++i){
                        shift_line_[i] = 0;
                    }
                }
            ~systolic_fir_sw() {
                delete[] shift_line_;
            }
            long fir(long input) {
                for(int i = TAPS-1 ; i > 0 ; --i ){
                    shift_line_[i] = shift_line_[i - 1];
                }

                shift_line_[0] = input;

                long output = 0;
                for (int i = 0; i < size_; i++ ){
                    output += coeff_[i] * shift_line_[i];
                }
                return output;
            }
        private:
            const long* coeff_;
            long* shift_line_;
            int size_;
    };

    systolic_fir_sw fir_filter(a, TAPS);
    for (int i = 0 ; i < N; i ++){
        sw[i] = fir_filter.fir(b[i]);
    }

    systolic_fir(a,b, c, hw); // calling DUT

    for(int j = 0; j < N; j++){
        std::cout << "Input " << b[j] << ", Expect " << sw[j] << ", Got " << hw[j] << std::endl;
        if (hw[j] != sw[j]) {
            std::cout << "FAILED" << std::endl;
            return EXIT_FAILURE;
        }
    }
    std::cout << "PASS" << std::endl;
    return EXIT_SUCCESS;
}

