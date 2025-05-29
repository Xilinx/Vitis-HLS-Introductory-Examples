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

