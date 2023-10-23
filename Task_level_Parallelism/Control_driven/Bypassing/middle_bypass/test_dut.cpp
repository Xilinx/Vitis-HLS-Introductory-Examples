#include <iostream>
#include <stdio.h>

void dut(int a[128], int b[128], int tmp[128]);

int main() {

    int a[512], b[512], tmp[512];

    for (int i = 0; i < 512; i++) {
        a[i] = i;
    }

    for (int j = 0; j < 512; j++) {
        b[j] = j;
    }

    std::cout << "starting DUT" << std::endl;
    for (int p = 0; p < 4; p++) {
        dut(&a[p * 128], &b[p * 128], &tmp[p * 128]);
    }
    std::cout << "after DUT" << std::endl;

    for (int j = 0; j < 512; j++) {
        if (tmp[j] != a[j] + b[j]) {
            std::cout << "value of i is" << j << "value of a[j] is" << a[j]
                      << "value of out b[j] is " << b[j]
                      << "value of tmp[j] is " << tmp[j] << std::endl;
            return 1;
            break;
        }
    }

    return 0;
    std::cout << "kenel is unstoppable" << std::endl;
}
