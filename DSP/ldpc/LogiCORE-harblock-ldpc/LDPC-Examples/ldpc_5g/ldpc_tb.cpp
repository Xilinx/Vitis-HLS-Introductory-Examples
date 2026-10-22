/*
 * Copyright 2026 Advanced Micro Devices, Inc. All Rights Reserved.
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
#include <iostream>
#include <cstdlib>
#include <vector>
#include "ldpc_top.h"
#include "hls_versal_ldpc_tb.h"

using namespace std;

// Control packet of a block. For the 5G standard the code is selected by its
// geometry: lifting size (z_j and z_set), base graph (bg) and number of parity
// rows of the base matrix (mb). The values below select a base graph 1 code with
// 46 parity rows (rate 1/3).
static void set_ctrl(ctrl_t &c, int id)
{
    c.set_z_j(4);
    c.set_z_set(0);
    c.set_bg(0);
    c.set_mb(46);
    c.set_hard_op(true);          // hard-decision output: 8 decoded bits for each byte of dout
    c.set_max_iterations(8);
    c.set_term_on_parity(true);   // stop the iterations when the parity checks pass
    // Algorithm fields. These values are the defaults of the static parameters. The IP
    // uses them for a block when the corresponding *_Source parameter is CTRL.
    c.set_algorithm(false);       // normalized min-sum
    c.set_alg_mod(12);            // scale = 12 x 0.0625 = 0.75
    c.set_nsat_bits(2);           // symmetric saturation of the LLRs to 6 bits
    c.set_max_schedule(0);
    c.set_id(id);                 // block identifier, passed through to the status packet
}

// The codeword is all zeros. The LLR of a bit equal to 0 is -4.
// A random subset of up to err_target bits is flipped to +4 (bit errors).
static int fill_block(vector<in_llr_t> &buf, unsigned n, unsigned err_target)
{
    buf.assign(n, (in_llr_t)-4);
    int errs = 0;
    for (unsigned i = 0; i < n; i++) {
        if (errs != (int)err_target && (rand() % 100) > 90) {
            buf[i] = (in_llr_t)4;
            errs++;
        }
    }
    return errs;
}

int main()
{
    srand(5);

    // Number of LLRs (N) and of information bits (K) of the code selected by the control packet
    unsigned N = 0, K = 0;
    {
        ctrl_t probe;
        set_ctrl(probe, 0);
        query_code_params<config>(probe, N, K);
    }
    cout << "5G (z_j=4, z_set=0, bg=0, mb=46): N=" << N << " K=" << K << endl;

    const int NBLK = 2;
    int fails = 0;
    vector<in_llr_t> buf;
    vector<out_t> out(N);

    // One block for each call of the top function
    for (int b = 0; b < NBLK; b++) {
        hls::stream<ctrl_t>            ctrl_s;
        hls::stream<din_bus<config>>   din_s;
        hls::stream<dout_bus<config>>  dout_s;
        hls::stream<status_t>          status_s;

        // Control packet and LLRs of the block (TLAST is set on the last din beat by din_pack)
        ctrl_t c;
        set_ctrl(c, b);
        ctrl_s.write(c);
        int in_errs = fill_block(buf, N, rand() % 30);
        din_pack<config>(buf.data(), N, config::DIN_Words, din_s, nullptr);

        // Simulate the decoder
        ldpc_top(ctrl_s, din_s, dout_s, status_s);

        // Status packet: the number of outputs is in num_op. The order of arrival of the
        // status and dout packets must not be assumed in a real system.
        status_t s = status_s.read();
        unsigned num_op = s.get_num_op();
        bool last_ok = false;
        unsigned keep_errs = 0;
        dout_unpack<config>(dout_s, num_op, config::DOUT_Words, out.data(), last_ok, keep_errs, /*hard*/ true);

        // All the decoded information bits must be 0
        unsigned bad = 0;
        for (unsigned i = 0; i < num_op && i < K; i++) {
            if (out[i] != (out_t)0)
                bad++;
        }
        cout << "block " << b << ": in_errs=" << in_errs
             << " num_op=" << num_op
             << " iters=" << s.get_dec_iterations()
             << " parity_pass=" << s.get_parity_pass()
             << " sys_bit_errors=" << bad
             << " tlast=" << last_ok
             << " keep_errs=" << keep_errs << endl;
        if (!s.get_parity_pass() || bad != 0 || !last_ok || keep_errs != 0)
            fails++;
    }

    cout << " ERRORS: " << fails << endl;
    if (fails > 0)
        cout << " (FAILED!!!)" << endl;
    else
        cout << " (PASSED!!!)" << endl;

    return fails > 0;
}
