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

#include "diamond.h"
#include <fstream>
#include <iostream>
using namespace std;

int main() {

    int totalNumWords = 512;
    std::vector<vecOf16Words> test(32);
    std::vector<vecOf16Words> outcome(32);

    int retval = 0;
    ofstream FILE;

    // Init test vector
    for (int i = 0; i < totalNumWords / 16; i++) {
        for (int j = 0; j < 16; j++) {
            test[i][j] = (data_t)j;
        }
    }
    cout << "Init: ";
    for (int i = 0; i < totalNumWords / 16; i++) {
        for (int j = 0; j < 16; j++) {
            cout << "  " << test[i][j];
        }
        cout << endl;
    }

    // Save the results to a file
    FILE.open("result.dat");

    // Executing the DUT thrice
    for (int iter = 0; iter < 3; iter++) {
        // Execute DUT
        diamond(test.data(), outcome.data(), totalNumWords / 16);

        // Display the results
        cout << "Outcome: ";
        for (int i = 0; i < totalNumWords / 16; i++) {
            for (int j = 0; j < 16; j++) {
                FILE << "  " << outcome[i][j];
            }
            FILE << endl;
        }
        FILE.close();
    }

    // Compare the results file with the golden results
    retval = system("diff --brief -w result.dat result.golden.dat");
    if (retval != 0) {
        cout << "Test failed  !!!" << endl;
        retval = 1;
    } else {
        cout << "Test passed !" << endl;
    }

    // Return 0 if the test passed
    return retval;
}
