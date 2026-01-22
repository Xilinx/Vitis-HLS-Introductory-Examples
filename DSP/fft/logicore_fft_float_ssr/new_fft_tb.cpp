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
#include "fft_top.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Test case structure
struct TestCase {
    const char* name;
    const char* input_file;
    const char* expected_file;
};

// Define the 5 test cases
TestCase test_cases[] = {
    {"Ramp",     "data/ramp_input.dat",     "data/ramp_fft_output.dat"},
    {"Sine",     "data/sine_input.dat",     "data/sine_fft_output.dat"},
    {"Sawtooth", "data/sawtooth_input.dat", "data/sawtooth_fft_output.dat"},
    {"Mixed",    "data/mixed_input.dat",    "data/mixed_fft_output.dat"},
    {"Impulse",  "data/impulse_input.dat",  "data/impulse_fft_output.dat"}
};

const int NUM_TESTS = sizeof(test_cases) / sizeof(TestCase);

// Function to read input data from file
bool read_input_data(const char* filename, cmpxDataIn data[FFT_LENGTH]) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "ERROR: Cannot open input file: " << filename << endl;
        return false;
    }
    
    for (int i = 0; i < FFT_LENGTH; i++) {
        float real_val, imag_val;
        if (!(infile >> real_val >> imag_val)) {
            cerr << "ERROR: Failed to read data at index " << i << " from " << filename << endl;
            infile.close();
            return false;
        }
        data[i] = complex<data_in_t>(real_val, imag_val);
    }
    
    infile.close();
    return true;
}

// Function to read expected FFT output from file
bool read_expected_data(const char* filename, cmpxDataOut data[FFT_LENGTH]) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "ERROR: Cannot open expected file: " << filename << endl;
        return false;
    }
    
    for (int i = 0; i < FFT_LENGTH; i++) {
        float real_val, imag_val;
        if (!(infile >> real_val >> imag_val)) {
            cerr << "ERROR: Failed to read data at index " << i << " from " << filename << endl;
            infile.close();
            return false;
        }
        data[i] = complex<data_out_t>(real_val, imag_val);
    }
    
    infile.close();
    return true;
}

// Function to compare FFT results with tolerance
bool compare_results(const char* test_name, cmpxDataOut computed[FFT_LENGTH], 
                     cmpxDataOut expected[FFT_LENGTH], float tolerance = 1e-1) {
    bool passed = true;
    int error_count = 0;
    float max_error = 0.0;
    int max_error_index = 0;
    
    for (int i = 0; i < FFT_LENGTH; i++) {
        float real_diff = abs(computed[i].real() - expected[i].real());
        float imag_diff = abs(computed[i].imag() - expected[i].imag());
        float total_error = sqrt(real_diff * real_diff + imag_diff * imag_diff);
        
        if (total_error > max_error) {
            max_error = total_error;
            max_error_index = i;
        }
        
        if (total_error > tolerance) {
            if (error_count < 10) {  // Print only first 10 errors
                cout << "  Mismatch at index " << i << ": ";
                cout << "computed = (" << computed[i].real() << ", " << computed[i].imag() << "), ";
                cout << "expected = (" << expected[i].real() << ", " << expected[i].imag() << "), ";
                cout << "error = " << total_error << endl;
            }
            error_count++;
            passed = false;
        }
    }
    
    if (passed) {
        cout << "  PASSED - Max error: " << max_error << " at index " << max_error_index << endl;
    } else {
        cout << "  FAILED - " << error_count << " mismatches found. Max error: " 
             << max_error << " at index " << max_error_index << endl;
    }
    
    return passed;
}

// Main testbench
int main() {
    int total_passed = 0;
    int total_failed = 0;
    
#if USE_STREAM_INTERFACE
    cout << "INFO: Using STREAM interface" << endl;
#else
    cout << "INFO: Using ARRAY interface" << endl;
#endif
    
    cout << "========================================" << endl;
    cout << "FFT Testbench - Testing " << NUM_TESTS << " cases" << endl;
    cout << "FFT Length: " << FFT_LENGTH << endl;
    cout << "SSR: " << FFT_SSR << endl;
    cout << "========================================" << endl << endl;
    
    // Run all test cases
    for (int t = 0; t < NUM_TESTS; t++) {
        cout << "Test " << (t + 1) << ": " << test_cases[t].name << endl;
        
        // Allocate arrays
        cmpxDataIn input[FFT_LENGTH];
        cmpxDataOut output[FFT_LENGTH];
        cmpxDataOut expected[FFT_LENGTH];
        bool ovflo;
        bool direction = false;  // Forward FFT
        
#if USE_STREAM_INTERFACE
        // Stream interface variables - use vectors for SSR>1
        hls::stream<hls::vector<cmpxDataIn, FFT_SSR>> in_stream;
        hls::stream<hls::vector<cmpxDataOut, FFT_SSR>> out_stream;
        hls::vector<cmpxDataIn, FFT_SSR> in_vec;
        hls::vector<cmpxDataOut, FFT_SSR> out_vec;
#endif
        
        // Read input data
        if (!read_input_data(test_cases[t].input_file, input)) {
            cout << "  FAILED - Cannot read input file" << endl;
            total_failed++;
            continue;
        }
        
        // Read expected output
        if (!read_expected_data(test_cases[t].expected_file, expected)) {
            cout << "  FAILED - Cannot read expected output file" << endl;
            total_failed++;
            continue;
        }
        
        // Initialize output
        for (int i = 0; i < FFT_LENGTH; i++) {
            output[i] = complex<data_out_t>(0.0, 0.0);
        }
        
        // Call DUT (Design Under Test)
#if USE_STREAM_INTERFACE
        // Stream interface: pack data into vectors and write to stream
        for (int i = 0; i < FFT_LENGTH; i += FFT_SSR) {
            for (int j = 0; j < FFT_SSR; j++) {
                in_vec[j] = input[i + j];
            }
            in_stream.write(in_vec);
        }
        fft_top(direction, in_stream, out_stream, &ovflo);
        // Read data from stream and unpack vectors
        for (int i = 0; i < FFT_LENGTH; i += FFT_SSR) {
            out_vec = out_stream.read();
            for (int j = 0; j < FFT_SSR; j++) {
                output[i + j] = out_vec[j];
            }
        }
#else
        // Array interface: pass arrays directly
        fft_top(direction, input, output, &ovflo);
#endif
        
        // Compare results
        if (compare_results(test_cases[t].name, output, expected)) {
            total_passed++;
        } else {
            total_failed++;
        }
        
        cout << endl;
    }
    
    // Print summary
    cout << "========================================" << endl;
    cout << "Test Summary" << endl;
    cout << "========================================" << endl;
    cout << "Total tests: " << NUM_TESTS << endl;
    cout << "Passed:      " << total_passed << endl;
    cout << "Failed:      " << total_failed << endl;
    
    if (total_failed == 0) {
#if USE_STREAM_INTERFACE
        cout << "\n*** STREAM INTERFACE: All tests passed successfully! ***\n";
#else
        cout << "\n*** ARRAY INTERFACE: All tests passed successfully! ***\n";
#endif
        return 0;
    } else {
        cout << "\n*** TEST FAILED: " << total_failed << " tests failed ***\n";
        return 1;
    }
}
