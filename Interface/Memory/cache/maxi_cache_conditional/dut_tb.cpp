#include <iostream>
#include <math.h>
#include <string.h>

#define N 1024

extern "C" void dut(const unsigned int* in, // Read-Only Vector 1
                    unsigned int* out,      // Output Result
                    int size                // Size in integer
);

void dut_sw(const unsigned int* in, // Read-Only Vector 1
            unsigned int* out,      // Output Result
            int size                // Size in integer
) {
    for (int i = 0; i < size; i++) {
        if (i != 512)
            out[i] = in[i] + 1;
        else
            out[i] = in[i];
    }
}

int main() {
    unsigned int A[N];
    unsigned int B[N], C[N];
    const unsigned int* in = A;
    unsigned int* out = B;
    unsigned int* ref = C;
    int i = 0;

    for (i = 0; i < N; i++)
        A[i] = i;

    for (i = 0; i < N; i++) {
        B[i] = 0;
        C[i] = 0;
    }

    dut(in, out, N);
    dut_sw(in, ref, N);

    int err = 0;

    for (i = 0; i < N; i++)
        if (ref[i] != out[i]) {
            std::cout << "index " << i << " ref = " << ref[i]
                      << " out = " << out[i] << std::endl;
            err++;
        }

    if (err) {
        std::cout << "FAIL" << std::endl;
        return -1;
    } else
        std::cout << "PASS" << std::endl;

    return 0;
}
