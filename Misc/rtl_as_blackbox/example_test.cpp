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

#include "example.h"
#include <iostream>

int main() {

    data_t sum;
    int i, j, retval = 0;
    ofstream FILE;

    // Save the results to a file
    FILE.open("result.dat");

    // Create input data
    for (i = 0; i < 10; ++i) {
        example(i, i - 10, i + 10, i - 20, i, 2 * i, 3 * i, 4 * i, sum);
        cout << " sum " << (int)sum << endl;
        FILE << sum << endl;
    }

    // Closing FILE
    FILE.close();

    // Compare the results file with the golden results
    retval = system("diff --brief -w result.dat result.golden.dat");
    if (retval != 0) {
        cout << "Test failed  !!!" << endl;
        retval = 1;
    } else {
        cout << "Test passed !" << endl;
    }

    // Return 0 if the test passes
    return retval;
}
