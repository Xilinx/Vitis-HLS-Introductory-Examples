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
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#define HLS_TASK_ALLOW_NON_STREAM_ARGS
#include "hls_print.h"
#include "hls_task.h"

void process_23(hls::stream<int>& in, hls::stream<int>& out) {
#pragma HLS INLINE off
    static int state = 0;
    static int val;

    in.read(val);
    val = val * 23;
    out.write(val);
}

void process_11(hls::stream<int>& in, hls::stream<int>& out) {
#pragma HLS INLINE off
    static int state = 0;
    static int val;

    in.read(val);
    val = val * 11;
    out.write(val);
}

void write_process(hls::stream<int>& in, hls::stream<int>& out, int* mem) {
    int val;
    static int addr = 0;

    in.read(val);
    if (addr >= 32)
        addr = 0;
    mem[addr] = val;
    addr++;
    val = mem[addr - 1];
    out.write(val);
}

extern "C" {

void stable_pointer(int* mem, hls::stream<int>& in, hls::stream<int>& out) {
#pragma HLS DATAFLOW
#pragma HLS INTERFACE mode = m_axi bundle = gmem depth =                       \
    256 max_read_burst_length = 16 max_widen_bitwidth =                        \
        512 max_write_burst_length = 16 num_read_outstanding =                 \
            16 num_write_outstanding = 16 port = mem
#pragma HLS stable variable = mem

    hls_thread_local hls::stream<int> int_fifo("int_fifo");
#pragma HLS STREAM depth = 512 type = fifo variable = int_fifo
    hls_thread_local hls::stream<int> int_fifo2("int_fifo2");
#pragma HLS STREAM depth = 512 type = fifo variable = int_fifo2

    hls_thread_local hls::task t1(process_23, in, int_fifo);
    hls_thread_local hls::task t2(process_11, int_fifo, int_fifo2);
    hls_thread_local hls::task t3(write_process, int_fifo2, out, mem);
}

} // extern "C"
