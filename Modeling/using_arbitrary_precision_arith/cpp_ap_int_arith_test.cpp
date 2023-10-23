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

#include "cpp_ap_int_arith.h"

int main() {
    dinA_t inA;
    dinB_t inB;
    dinC_t inC;
    dinD_t inD;
    dout1_t out1;
    dout2_t out2;
    dout3_t out3;
    dout4_t out4;

    int i, retval = 0;
    FILE* fp;

    // Save the results to a file
    fp = fopen("result.dat", "w");

    for (i = 0; i < N; i++) {
        // Create input data
        inA = i + 2;
        inB = i + 23;
        inC = i + 234;
        inD = i + 2345;

        // Call the function to operate on the data
        cpp_ap_int_arith(inA, inB, inC, inD, &out1, &out2, &out3, &out4);

        fprintf(fp, "%d*%d=%d; %d+%d=%d; %d/%d=%d; %d mod %d=%d;\n",
                inA.to_int(), inB.to_int(), out1.to_int(), inB.to_int(),
                inA.to_int(), out2.to_int(), inC.to_int(), inA.to_int(),
                out3.to_int(), inD.to_int(), inA.to_int(), out4.to_int());
    }
    fclose(fp);

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
