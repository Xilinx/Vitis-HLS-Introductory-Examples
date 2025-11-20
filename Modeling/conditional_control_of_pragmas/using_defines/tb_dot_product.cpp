#include <iostream>
#include "dot_product.h"

int main() {
    int A[SIZE], B[SIZE];
    int expected = 0;

    // Initialize arrays with sample values
    for (int i = 0; i < SIZE; i++) {
        A[i] = i;
        B[i] = SIZE - i;
        expected += A[i] * B[i];
    }

    int result = dot_product(A, B);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Expected: " << expected << std::endl;

    if (result == expected) {
        std::cout << "Test PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "Test FAILED" << std::endl;
        return 1;
    }
}

