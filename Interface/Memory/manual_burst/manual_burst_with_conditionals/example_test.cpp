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

void example_sw(din_t A[N], din_t B[N], dout_t RES[N], din_t factor) {

    dout_t X_accum = 0;
    dout_t Y_accum = 0;
    int i, j;
    dout_t x_aux[N];
    dout_t y_aux[N];

SUM_X0:
    for (i = 0; i < N / 4; i++) {
        X_accum += A[i];
        x_aux[i] = X_accum;
    }
SUM_Y0:
    for (i = 0; i < N / 4; i++) {
        Y_accum += B[i];
        y_aux[i] = Y_accum;
    }

    for (int i = 0; i < N / 4; i++) {
        x_aux[i] = factor + x_aux[i];
        y_aux[i] = factor + y_aux[i];
    }

MUL0:
    for (i = 0; i < N / 4; i++) {
        RES[i] = (x_aux[i] - y_aux[i]);
    }

    X_accum = N / 4;

SUM_X16:
    for (i = N - N / 4; i < N; i++) {
        X_accum += A[i];
        x_aux[i] = X_accum - N;
    }
    Y_accum = N / 4;
SUM_Y16:
    for (i = N - N / 4; i < N; i++) {
        Y_accum += B[i];
        y_aux[i] = Y_accum + N;
    }

    for (int i = N - N / 4; i < N; i++) {
        x_aux[i] = x_aux[i] - factor;
        y_aux[i] = y_aux[i] - factor;
    }

MUL16:
    for (i = N - N / 4; i < N; i++) {
        RES[i] = (x_aux[i] - y_aux[i] / N);
    }
}

int main() {
    din_t A[N];
    din_t B[N];
    dout_t RES[N];
    din_t A_sw[N], B_sw[N];
    dout_t RES_sw[N];
    din_t factor = N;

    int i, errval = N / 2;

    // Create input data
    for (i = 0; i < N; ++i) {
        A[i] = N / 4 - i;
        B[i] = N - 1 - i;
        A_sw[i] = A[i];
        B_sw[i] = B[i];
    }

    // Call the HW function:
    example(A, B, RES, factor);

    // Call the SW function:
    example_sw(A, B, RES_sw, factor);

    // Compare the results file with the golden results:
    for (i = 0; i < N / 4; ++i) {
        if (RES[i] != RES_sw[i])
            printf("\n ERROR (%d) expected (int): %d != %d found", i, RES_sw[i],
                   RES[i]);
        else
            errval--;
    }
    for (i = N - N / 4; i < N; ++i) {
        if (RES[i] != RES_sw[i])
            printf("\n ERROR (%d) expected (int): %d != %d found", i, RES_sw[i],
                   RES[i]);
        else
            errval--;
    }

    // Return the final test result:
    if (errval > 0)
        printf("\n golden mismatch \n");
    else
        printf("\n Test passed !\n");
    // Return 0 if the test passed
    return 0;
}
