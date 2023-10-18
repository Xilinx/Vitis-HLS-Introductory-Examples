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

#include "hier_func4.h"

int main() {
    // Data storage
    int a[NUM_TRANS], b[NUM_TRANS];
    int c_expected[NUM_TRANS], d_expected[NUM_TRANS];
    int c[NUM_TRANS], d[NUM_TRANS];

    // Function data (to/from function)
    int a_actual, b_actual;
    int c_actual, d_actual;

    // Misc
    int retval = 0, i, i_trans, tmp;
    FILE* fp;

    // Load input data from files
    fp = fopen("tb_data/inA.dat", "r");
    for (i = 0; i < NUM_TRANS; i++) {
        fscanf(fp, "%d", &tmp);
        a[i] = tmp;
    }
    fclose(fp);

    fp = fopen("tb_data/inB.dat", "r");
    for (i = 0; i < NUM_TRANS; i++) {
        fscanf(fp, "%d", &tmp);
        b[i] = tmp;
    }
    fclose(fp);

    // Execute the function multiple times (multiple transactions)
    for (i_trans = 0; i_trans < NUM_TRANS - 1; i_trans++) {

        // Apply next data values
        a_actual = a[i_trans];
        b_actual = b[i_trans];

        hier_func4(a_actual, b_actual, &c_actual, &d_actual);

        // Store outputs
        c[i_trans] = c_actual;
        d[i_trans] = d_actual;
    }

    // Load expected output data from files
    fp = fopen("tb_data/outC.golden.dat", "r");
    for (i = 0; i < NUM_TRANS; i++) {
        fscanf(fp, "%d", &tmp);
        c_expected[i] = tmp;
    }
    fclose(fp);

    fp = fopen("tb_data/outD.golden.dat", "r");
    for (i = 0; i < NUM_TRANS; i++) {
        fscanf(fp, "%d", &tmp);
        d_expected[i] = tmp;
    }
    fclose(fp);

    // Check outputs against expected
    for (i = 0; i < NUM_TRANS - 1; ++i) {
        if (c[i] != c_expected[i]) {
            retval = 1;
        }
        if (d[i] != d_expected[i]) {
            retval = 1;
        }
    }

    // Print Results
    if (retval == 0) {
        printf("    *** *** *** *** \n");
        printf("    Results are good \n");
        printf("    *** *** *** *** \n");
    } else {
        printf("    *** *** *** *** \n");
        printf("    Mismatch: retval=%d \n", retval);
        printf("    *** *** *** *** \n");
    }

    // Return 0 if outputs are correct
    return retval;
}
