/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
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

#include "ecc_flags.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
    din_t in1[N];
    din_t in2[N];
    din_t output[N];
    const din_t Iter = 100;

    int i, retval = 0;
    ofstream FILE;

    // Create input data
    for (i = 0; i < N; ++i) {
        in1[i] = 1000 * i;
        in2[i] = 1000 * i;
        cout << in1[i] << endl;
        cout << in2[i] << endl;
    }
    // Save the results to a file
    FILE.open("result.dat");

    // Call the function

    ecc_flags(in1, in2, Iter, output);

    // Save output data
    for (i = 0; i < N; ++i) {
        cout << output[i] << endl;
        FILE << output[i] << endl;
    }
    FILE.close();

    // Compare the results file with the golden results
    retval = system("diff --brief -w result.dat result.golden.dat");
    if (retval != 0) {
        printf("Test failed  !!!\n");
        retval = 1;
    } else {
        printf("Test passed !\n");
    }

    // Return 0 if the test passes
    return retval;
}
