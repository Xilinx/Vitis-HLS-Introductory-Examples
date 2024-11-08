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

