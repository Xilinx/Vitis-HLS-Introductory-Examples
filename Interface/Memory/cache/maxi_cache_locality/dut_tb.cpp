#include <iostream>
#include <math.h>
#include <string.h>

#define N 1024

extern "C" void dut(const double* in, // Read-Only Vector 1
                    double* out,      // Output Result
                    int size          // Size in integer
);

void dut_sw(const double* in, // Read-Only Vector 1
            double* out,      // Output Result
            int size          // Size in integer
) {
    for (int i = 0; i < size; i++) {
        out[i] = in[i] + in[i + 1];
    }
}

int main() {
    double A[N + 1];
    double B[N + 1], C[N + 1];
    const double* in = A;
    double* out = B;
    double* ref = C;
    int i = 0;

    for (i = 0; i < N + 1; i++) {
        A[i] = i;
        B[i] = 0;
        C[i] = 0;
    }

    dut(in, out, N);
    dut_sw(in, ref, N);

    int err = 0;

    for (i = 0; i < N; i++)
        if (fabs(out[i] / ref[i] - 1) > 1e-6) {
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
