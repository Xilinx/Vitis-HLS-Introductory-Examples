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
#include "test.h"
#include "etc/ap_utils.h"
#include <iostream>

void write(hls::stream<int> &inputStream, int A[N][N], hls::stream<int> &line) {
#pragma HLS INLINE OFF
    
    for (unsigned int i=0; i < N; ++i) {
#pragma HLS PIPELINE II=3
        for (unsigned int j=0; j < N; ++j) {
            int temp = inputStream.read();
            std::cout << "Write: Reading A[" << i << "][" << j << "] = " << temp << std::endl;
            A[i][j] = temp;
        }

        // This is to create a cycle delay
        ap_wait();
        // This stream acts like a barrier to prevent the read process from
        // starting to read a row early. 
        line.write(i);
    }
    
}

void read(int A[N][N], hls::stream<int> &outputStream, hls::stream<int> &line)
{
#pragma HLS INLINE OFF 
    int row;
    do {
#pragma HLS PIPELINE II=3
        row = line.read(); // This gates this process. 
        for (unsigned j=0; j < N; ++j) {
            int temp = A[row][j];
            std::cout << "Read: Writing A[" << row << "][" << j << "] = " << temp << std::endl;
            outputStream << temp;
        }
    } while (row < N-1);
}

void dut(hls::stream<int> &inputStream, int A[N][N], hls::stream<int> &outputStream) {
#pragma HLS INTERFACE axis  port=inputStream
#pragma HLS INTERFACE m_axi port=A 
#pragma HLS INTERFACE axis  port=outputStream
#pragma HLS DATAFLOW
    hls::stream<int> line;

    write(inputStream, A, line);
    read(A, outputStream, line);
}
