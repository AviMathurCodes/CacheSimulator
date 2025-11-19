/*

* Copyright (c) 2025, Shiv Nadar University, Delhi NCR, India. All Rights

* Reserved. Permission to use, copy, modify and distribute this software for

* educational, research, and not-for-profit purposes, without fee and without a

* signed license agreement, is hereby granted, provided that this paragraph and

* the following two paragraphs appear in all copies, modifications, and

* distributions.

*

* IN NO EVENT SHALL SHIV NADAR UNIVERSITY BE LIABLE TO ANY PARTY FOR DIRECT,

* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST

* PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE.

*

* SHIV NADAR UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT

* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A

* PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS PROVIDED "AS IS". SHIV

* NADAR UNIVERSITY HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,

* ENHANCEMENTS, OR MODIFICATIONS.

*/

 

/*

Revision History:

Date          By                     Change Notes
13 Nov 2025   Avi Mathur             Completed Cache class implementation
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"
#include "Cache.h"
#include "MainMemory.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <config-file> <memops-file> [-v]\n";
        return 1;
    }
    bool verbose = (argc >= 4 && string(argv[3]) == "-v");

    // ---------- Read config file (allow // comments, blank lines, inline comments) ----------
    ifstream cfg(argv[1]);
    if (!cfg) { cerr << "Cannot open config file\n"; return 1; }

    vector<uint64_t> cfgvals;
    string line;
    while (getline(cfg, line) && cfgvals.size() < 6) {
        // remove comments and trim
        string without_comment = trim(remove_cpp_comment(line));
        if (without_comment.empty()) continue;
        // Split into tokens (in case someone put multiple numbers on a line)
        auto toks = split_whitespace(without_comment);
        for (auto &t : toks) {
            // try to parse integer
            try {
                uint64_t v = stoull(t, nullptr, 0);
                cfgvals.push_back(v);
            } catch (...) {
                // ignore tokens that are not numbers (be lenient)
            }
            if (cfgvals.size() >= 6) break;
        }
    }
    cfg.close();

    if (cfgvals.size() < 6) {
        cerr << "Config file must contain 6 numeric values (L1_size, L1_block, L1_ways, L2_size, L2_block, L2_ways). You may use // comments.\n";
        return 1;
    }

    int L1_size   = (int)cfgvals[0];
    int L1_block  = (int)cfgvals[1];
    int L1_ways   = (int)cfgvals[2];
    int L2_size   = (int)cfgvals[3];
    int L2_block  = (int)cfgvals[4];
    int L2_ways   = (int)cfgvals[5];

    Cache L1("L1", L1_size, L1_block, L1_ways);
    Cache L2("L2", L2_size, L2_block, L2_ways);
    MainMemory::init();

    ifstream memfile(argv[2]);
    if (!memfile) { cerr << "Cannot open memops file\n"; return 1; }

    uint64_t oob_ops = 0;
    while (getline(memfile, line)) {
        // remove comments anywhere on the line (//) and trim
        string cleaned = trim(remove_cpp_comment(line));
        if (cleaned.empty()) continue; // skip comments / blank lines

        auto parts = split_whitespace(cleaned);
        if (parts.size() < 2) continue;

        char op = toupper(parts[0][0]);
        uint64_t addr = 0;
        try {
            addr = stoull(parts[1], nullptr, 0);
        } catch (...) {
            cout << "Bad address token: " << parts[1] << " -> skipping line\n";
            continue;
        }

        if (!MainMemory::in_bounds(addr)) { cout << op << " " << parts[1] << " -> OOB\n"; oob_ops++; continue; }

        if (op == 'R') {
            string path;
            uint8_t val = L1.read_byte((uint32_t)addr, &L2, path);
            cout << "R " << parts[1] << " -> " << path << " | value=" << (unsigned)val << "\n";
        } else if (op == 'W' && parts.size() >= 3) {
            unsigned v = 0;
            try {
                v = stoul(parts[2]);
            } catch (...) {
                cout << "Bad write value: " << parts[2] << " -> skipping\n";
                continue;
            }
            if (v > 255) { cout << "value must be 0..255\n"; continue; }
            L1.write_byte((uint32_t)addr, &L2, (uint8_t)v);
            cout << "W " << parts[1] << " = " << v << " -> OK\n";
        } else {
            // Unknown op - ignore
            cout << "Unknown op: " << parts[0] << " - skipping\n";
        }

        if (verbose) {
            cout << "=== After this operation (verbose) ===\n";
            L1.debug_print(); L2.debug_print(); MainMemory::print_state();
            cout << "=====================================\n";
        }
    }

    cout << "\n=== Final Summary ===\n";
    L1.debug_print(); L2.debug_print(); MainMemory::print_state();
    cout << "OOB ops: " << oob_ops << "\n";
    return 0;
}


