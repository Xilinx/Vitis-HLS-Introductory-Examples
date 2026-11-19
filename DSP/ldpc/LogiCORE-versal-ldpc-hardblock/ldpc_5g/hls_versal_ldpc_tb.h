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
#ifndef X_HLS_VERSAL_LDPC_TB_H
#define X_HLS_VERSAL_LDPC_TB_H
//
// Versal LDPC Decoder - OPTIONAL testbench helpers (not part of the library).
//
// These functions pack a block of LLRs into the DIN beats of the IP, drive the
// DIN_WORDS and DOUT_WORDS streams, and unpack the DOUT beats. They use the byte
// layout of the hardware IP, so that C simulation and RTL simulation see the same
// transactions. They are used by the testbench only, not by the design.
//
// They cover the common cases (the same number of words in every beat, or a
// functor nm(beat, lane) that returns the number of words of a lane in a beat).
// For an irregular pattern, drive the streams directly: din_bus, dout_bus,
// din_words_t and dout_words_t are public structs, and lane_bit_offset() (in
// hls_versal_ldpc.h) gives the position of an element in a bus word.
//
#include "hls_versal_ldpc.h"

namespace hls {
namespace versal_ldpc {

// Pack `n` LLRs (codeword order) into DIN beats. Drive DIN_WORDS unless the
// words configuration is Fixed (words_s is then not used and can be nullptr).
// Functor form: nm(beat, lane) returns the nominal number of words of a lane in a beat.
template <typename CFG, typename NMFN>
void din_pack(const in_llr_t* in, unsigned n, NMFN nm,
              hls::stream<din_bus<CFG>>&     din_s,
              hls::stream<din_words_t<CFG>>* words_s) {
    const unsigned lanes = CFG::DIN_Lanes;
    const unsigned lbits = din_bus<CFG>::WIDTH / lanes;
    if (words_s && CFG::DIN_Words_Configuration == WORDS_PER_BLOCK) {
        din_words_t<CFG> wv; wv.data = 0; wv.data.range(7, 0) = nm(0, 0); wv.last = true;
        words_s->write(wv);   // one beat per block -> TLAST always 1
    }
    unsigned idx = 0, remaining = n, beat = 0;
    while (remaining > 0) {
        din_bus<CFG> b; b.data = 0; b.last = false;
        din_words_t<CFG> wv; wv.data = 0;
        unsigned rem_beat = remaining;
        for (unsigned L = 0; L < lanes; L++) {
            unsigned want = nm(beat, L);
            unsigned w = (want < rem_beat) ? want : rem_beat;
            for (unsigned p = 0; p < w; p++) {
                unsigned off = lane_bit_offset(L, p, lbits);
                in_llr_t v = in[idx++];
                b.data.range(off + 7, off) = v.range(7, 0);
            }
            rem_beat -= w;
            wv.data.range(8 * L + 7, 8 * L) = want;   // nominal; the core caps it to the remaining words
        }
        remaining = rem_beat;
        b.last = (remaining == 0);
        din_s.write(b);
        if (words_s && CFG::DIN_Words_Configuration == WORDS_PER_TRANSACTION) {
            wv.last = (remaining == 0);
            words_s->write(wv);
        }
        beat++;
    }
}
template <typename CFG>
void din_pack(const in_llr_t* in, unsigned n, unsigned nm,
              hls::stream<din_bus<CFG>>&     din_s,
              hls::stream<din_words_t<CFG>>* words_s) {
    din_pack<CFG>(in, n, [nm](unsigned, unsigned) { return nm; }, din_s, words_s);
}

// Drive the DOUT_WORDS input for a block of `num_op` outputs.
template <typename CFG, typename NMFN>
void dout_words_drive(unsigned num_op, NMFN nm, hls::stream<dout_words_t<CFG>>& words_s) {
    const unsigned lanes = CFG::DOUT_Lanes;
    if (CFG::DOUT_Words_Configuration == WORDS_PER_BLOCK) {
        dout_words_t<CFG> wv; wv.data = 0; wv.data.range(7, 0) = nm(0, 0); wv.last = true;
        words_s.write(wv);   // one beat per block -> TLAST always 1
    } else if (CFG::DOUT_Words_Configuration == WORDS_PER_TRANSACTION) {
        unsigned remaining = num_op, beat = 0;
        while (remaining > 0) {
            dout_words_t<CFG> wv; wv.data = 0;
            unsigned rem_beat = remaining;
            for (unsigned L = 0; L < lanes; L++) {
                unsigned want = nm(beat, L);
                wv.data.range(8 * L + 7, 8 * L) = want;          // nominal; the core caps it
                rem_beat -= (want < rem_beat) ? want : rem_beat;
            }
            remaining = rem_beat;
            wv.last = (remaining == 0);
            words_s.write(wv);
            beat++;
        }
    }
}
template <typename CFG>
void dout_words_drive(unsigned num_op, unsigned nm, hls::stream<dout_words_t<CFG>>& words_s) {
    dout_words_drive<CFG>(num_op, [nm](unsigned, unsigned) { return nm; }, words_s);
}

// Unpack `num_op` outputs (LLRs, or hard bits when hard = true) from the DOUT beats,
// check TKEEP and return TLAST. In hard mode the IP packs 8 hard bits per byte
// (least significant bit first), so DOUT carries ceil(num_op/8) bytes; the numbers
// of words per beat and lane (nm) are byte counts in both modes. out[] receives one
// entry for each value.
template <typename CFG, typename NMFN>
unsigned dout_unpack(hls::stream<dout_bus<CFG>>& dout_s, unsigned num_op, NMFN nm,
                     out_t* out, bool& last_ok, unsigned& keep_errs, bool hard = false) {
    const unsigned lanes = CFG::DOUT_Lanes;
    const unsigned lbits = dout_bus<CFG>::WIDTH / lanes;
    const unsigned nbytes = hard ? ((num_op + 7) / 8) : num_op;   // DOUT byte count
    unsigned vidx = 0, rbytes = nbytes, beat = 0;
    keep_errs = 0;
    dout_bus<CFG> b; b.last = false;
    while (rbytes > 0) {
        b = dout_s.read();
        unsigned rem_beat = rbytes;
        for (unsigned L = 0; L < lanes; L++) {
            unsigned want = nm(beat, L);
            unsigned w = (want < rem_beat) ? want : rem_beat;
            for (unsigned p = 0; p < w; p++) {
                unsigned off = lane_bit_offset(L, p, lbits);
                if (b.keep[off / 8] != 1) keep_errs++;
                ap_uint<8> byte = b.data.range(off + 7, off);
                if (hard) {
                    for (unsigned bit = 0; bit < 8 && vidx < num_op; ++bit)
                        out[vidx++] = (out_t)(byte[bit] ? 1 : 0);   // LSB first
                } else {
                    out_t o; o.range(7, 0) = byte; out[vidx++] = o;
                }
            }
            rem_beat -= w;
        }
        rbytes = rem_beat;
        beat++;
    }
    last_ok = b.last;
    return vidx;
}
template <typename CFG>
unsigned dout_unpack(hls::stream<dout_bus<CFG>>& dout_s, unsigned num_op, unsigned nm,
                     out_t* out, bool& last_ok, unsigned& keep_errs, bool hard = false) {
    return dout_unpack<CFG>(dout_s, num_op, [nm](unsigned, unsigned) { return nm; },
                            out, last_ok, keep_errs, hard);
}

} // namespace versal_ldpc
} // namespace hls

#endif // X_HLS_VERSAL_LDPC_TB_H
