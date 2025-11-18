#include <iostream>
#include "adder.h"

int main() {
    int a[10], b[10], c[10], z[10], expected[10];
    
    // Initialize test data
    for (int i = 0; i < 10; i++) {
        a[i] = i + 1;        // 1, 2, 3, ..., 10
        b[i] = (i + 1) * 2;  // 2, 4, 6, ..., 20
        c[i] = (i + 1) * 3;  // 3, 6, 9, ..., 30
    }
    
    // Run the DUT
    top(a, b, c, z);
    
    // Compute expected results (z = a + b + c)
    int tmp[10];
    for (int i = 0; i < 10; i++) {
        tmp[i] = a[i] + b[i];
        expected[i] = c[i] + tmp[i];
    }
    
    // Print results and verify
    std::cout << "Vitis HLS Adder Test" << std::endl;
    std::cout << "Input A: ";
    for (int i = 0; i < 10; i++) std::cout << a[i] << " ";
    std::cout << std::endl;
    
    std::cout << "Input B: ";
    for (int i = 0; i < 10; i++) std::cout << b[i] << " ";
    std::cout << std::endl;
    
    std::cout << "Input C: ";
    for (int i = 0; i < 10; i++) std::cout << c[i] << " ";
    std::cout << std::endl;
    
    std::cout << "Result:  ";
    for (int i = 0; i < 10; i++) std::cout << z[i] << " ";
    std::cout << std::endl;
    
    // Verify results
    bool pass = true;
    for (int i = 0; i < 10; i++) {
        if (z[i] != expected[i]) {
            std::cout << "MISMATCH at index " << i << ": result=" << z[i] << ", expected=" << expected[i] << std::endl;
            pass = false;
        }
    }
    
    std::cout << "Test Result: " << (pass ? "PASS" : "FAIL") << std::endl;
    
    return pass ? 0 : 1;
}