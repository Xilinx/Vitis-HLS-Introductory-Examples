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

#include "free_pipe_mult.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

int main() {

    data_t Atest[SZ];
    data_t total;
    stream<data_t> strm;

    int retval = 0;
    ofstream FILE;

    // Create input data
    for (int i = 0; i < SZ; ++i) {
        Atest[i] = i;
        strm << i;
        cout << Atest[i] << endl;
    }
    // Save the results to a file
    FILE.open("result.dat");

    // Call the function
    free_pipe_mult(Atest, strm, &total);

    // Save output data
    cout << "Result: " << total << endl;
    FILE << total << endl;
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
