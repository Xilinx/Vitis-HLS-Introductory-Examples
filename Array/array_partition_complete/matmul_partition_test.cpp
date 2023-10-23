/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

// OpenCL utility layer include
#include <algorithm>
#include <cstdio>
#include <random>
#include <vector>
#include <iomanip>
#include <iostream>
#include "matmul_partition.h"

using std::default_random_engine;
using std::generate;
using std::uniform_int_distribution;
using std::vector;

void matmul(int* C, int* A, int* B, int M) {
    for (int k = 0; k < M; k++) {
        for (int j = 0; j < M; j++) {
            for (int i = 0; i < M; i++) {
                C[k * M + j] += A[k * M + i] * B[i * M + j];
            }
        }
    }
}

int gen_random() {
    static default_random_engine e;
    static uniform_int_distribution<int> dist(0, 10);

    return dist(e);
}

void print(int* data, int columns, int rows) {
    vector<int> out(columns * rows);
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            std::cout << std::setw(4) << data[r * columns + c] << " ";
        }
        std::cout << "…\n";
    }
    for (int r = 0; r < 10; r++) {
        std::cout << "   … ";
    }
    std::cout << "⋱\n\n";
}

void verify(vector<int>& gold, vector<int>& output) {
    for (int i = 0; i < (int)output.size(); i++) {
        if (output[i] != gold[i]) {
            std::cout << "Mismatch " << i << ": gold: " << gold[i] << " device: " << output[i] << "\n";
            print(output.data(), 16, 16);
            exit(EXIT_FAILURE);
        }
    }
}

// This example illustrates how to use array partitioning attributes in HLS
// kernels for FPGA devices using matmul.
int main() {

    static const int columns = 16;
    static const int rows = 16;
    vector<int> A(columns * rows);
    vector<int> B(columns * rows);
    vector<int> gold1(columns * rows, 0);
    vector<int> C(columns * rows, 0);
    vector<int> gold2(columns * rows, 0);

    generate(begin(A), end(A), gen_random);
    generate(begin(B), end(B), gen_random);

    std::cout << "A:\n";
    print(A.data(), columns, rows);

    std::cout << "B:\n";
    print(B.data(), columns, rows);

    matmul(gold1.data(), A.data(), B.data(), columns);
    std::cout << "Gold1:\n";
    print(gold1.data(), columns, rows);

    matmul_partition(A.data(), B.data(), C.data(), columns, columns);

    verify(gold1, C);

    std::cout << "TEST PASSED\n\n";

    return EXIT_SUCCESS;
}
