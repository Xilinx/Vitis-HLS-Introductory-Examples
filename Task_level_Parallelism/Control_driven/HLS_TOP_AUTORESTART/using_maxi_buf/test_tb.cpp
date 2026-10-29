/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#ifndef __SYNTHESIS__
#include <thread>
#include <chrono>
#include <cstdio>
#endif

#ifndef __SYNTHESIS__
std::chrono::steady_clock::time_point T0;

long ms() {
    return (long)std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - T0).count();
}

void kernel_sleep_chunk(int i) {
    if (i && (i % CHUNK) == 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(CHUNK_MS));
}

// Deliberate, readable push order -- not a plain cycle, so a
// dropped or reordered offset is obvious in the output.
static const int SEQ[M] = { 0, 1, 2, 3, 1, 0, 3, 2 };

void updater(hls::m_axi_buf<int, hls::ap_none<size_t>> *buf, const size_t *off) {
    for (int m = 0; m < M; m++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(PUSH_MS));
        printf("[%5ld ms][updater] push #%d offset=%-6ld region=%d\n",
               ms(), m, (long)off[SEQ[m]], SEQ[m]);
        fflush(stdout);
        buf->write_offset(off[SEQ[m]]);
    }
}

int main() {
    T0 = std::chrono::steady_clock::now();

    hls::m_axi_buf<int, hls::ap_none<size_t>> buf;
    hls::stream<int> out;

    static int src[REGIONS][N];
    size_t off[REGIONS];
    for (int k = 0; k < REGIONS; k++) {
        for (int i = 0; i < N; i++) src[k][i] = k * TAG + i;
        off[k] = buf.allocate(src[k], N);
        printf("[%5ld ms][main   ] region %d @ offset %-6ld values %d..%d\n",
               ms(), k, (long)off[k], src[k][0], src[k][N - 1]);
    }
    printf("[%5ld ms][main   ] ONE invocation, %d samples total, "
           "read_offset() x%d, ~%d ms/block vs %d ms/push\n",
           ms(), TOTAL, M, CHUNK_MS * (N / CHUNK), PUSH_MS);

    // count = 1 -> a SINGLE kernel invocation.  Nothing is queued
    // yet, so the kernel starts and blocks inside the first
    // read_offset() until the updater thread pushes.  launch() is the
    // threaded form and returns immediately.
    my_dut t;
    t.launch(1, buf, out);
    printf("[%5ld ms][main   ] launch(1) returned -- kernel running\n", ms());

    std::thread up(updater, &buf, off);

    // ---- drain live, one block at a time ------------------------------
    int observed[M];
    int torn = 0;
    for (int b = 0; b < M; b++) {
        int first = out.read();
        int region = first / TAG;
        int mixed = 0, lo = region, hi = region;

        for (int i = 1; i < N; i++) {
            int r = out.read() / TAG;
            if (r != region) { mixed = 1; if (r < lo) lo = r; if (r > hi) hi = r; }
        }

        observed[b] = region;
        printf("[%5ld ms][main   ] block %d -> region %d  (%s)\n",
               ms(), b, region,
               mixed ? "*** MIXED REGIONS IN ONE BLOCK ***" : "single region, clean");
        fflush(stdout);
        if (mixed) { torn++; (void)lo; (void)hi; }
    }

    up.join();
    t.wait_for_done();

    // ---- report, do not assert ---------------------------------------
    printf("\npushed  :");
    for (int m = 0; m < M; m++) printf(" %d", SEQ[m]);
    printf("\nobserved:");
    for (int m = 0; m < M; m++) printf(" %d", observed[m]);
    printf("\n");

    int inorder = 1;
    for (int m = 0; m < M; m++) if (observed[m] != SEQ[m]) inorder = 0;

    printf("\n  sequence : %s\n", inorder
           ? "every read_offset() picked up the offset pushed for it, in order"
           : "DIVERGED from the pushed order (expected once updates are async)");
    printf("  tearing  : %s\n", torn
           ? "*** at least one block mixed regions -- read_offset() TORE ***"
           : "none -- each block stayed on one region end to end");

    return 0;   // observation test: never fails
}
#endif
