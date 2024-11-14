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
#include "transposed_fir.h"
#include <iostream>

int main(void) {
    long h0=1,h1=2,h2=4,h3=8,hsum=h0+h1+h2+h3,errors=0;
    const int num=30;
    std::cout << "(i) init" << std::endl;

    for (int i = 0; i < num; i++) {
        long out,in;
        switch(i) {
            case 0: in=1; break;
            case num/3: in=10;break;
            case 2*num/3: in=100;break;
            default: in=0;
        }
        if (i>3*num/4) {
            in=i;
        }
        long expected= in*hsum;
        transposed_fir(out,in,h0,h1,h2,h3);
        std::cout << "(i) iteration "<<i<<" in "<<in<<" out " << out;
        if(out==expected) {
            std::cout << " (ok)" << std::endl;
        } else {
            errors++;
            std::cout << " expected " << expected << " error "<<errors << std::endl;
        }
    }
    if(errors>0) {
        std::cout << "(!) C-TB FAILED" << std::endl;
    } else {
        std::cout << "(i) C-TB PASS" << std::endl;
    }
    return errors;
}

