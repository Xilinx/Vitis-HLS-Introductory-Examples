/**
 * Copyright (C) 2022 Xilinx, Inc
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

#include "krnl_vmult.hpp"

// Read Data from Global Memory and write into Stream inStream
static void read_input(uint32_t* in, hls::stream<uint32_t>& inStream,
                       int vSize) {
// Auto-pipeline is going to apply pipeline to this loop
mem_rd:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking write command to inStream
        inStream << in[i];
    }
}

// Read Input data from inStream and write the result into outStream
static void compute_mult(hls::stream<uint32_t>& inStream1,
                         hls::stream<uint32_t>& inStream2,
                         hls::stream<uint32_t>& outStream, int vSize) {
// Auto-pipeline is going to apply pipeline to this loop
execute:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking read command from inStream and Blocking write command
        // to outStream
        outStream << (inStream1.read() * inStream2.read());
    }
}

// Read result from outStream and write the result to Global Memory
static void write_result(uint32_t* out, hls::stream<uint32_t>& outStream,
                         int vSize) {
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking read command to inStream
        out[i] = outStream.read();
    }
}

extern "C" {
/*
    Vector Multiplication Kernel Implementation using dataflow
    Arguments:
        in1   (input)  --> Input Vector 1
        in2   (input)  --> Input Vector 2
        out  (output) --> Output Vector
        vSize (input)  --> Size of Vector in Integer
   */
void krnl_vmult(uint32_t* in1, uint32_t* in2, uint32_t* out, int vSize) {
    static hls::stream<uint32_t> inStream1("input_stream_1");
    static hls::stream<uint32_t> inStream2("input_stream_2");
    static hls::stream<uint32_t> outStream("output_stream");
#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0 depth = 4096
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1 depth = 4096
#pragma HLS INTERFACE m_axi port = out bundle = gmem0 depth = 4096

#pragma HLS dataflow
    // dataflow pragma instruct compiler to run following three APIs in parallel
    read_input(in1, inStream1, vSize);
    read_input(in2, inStream2, vSize);
    compute_mult(inStream1, inStream2, outStream, vSize);
    write_result(out, outStream, vSize);
}
}
