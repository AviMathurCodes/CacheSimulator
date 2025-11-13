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

    ifstream cfg(argv[1]);
    if (!cfg) { cerr << "Cannot open config file\n"; return 1; }
    int L1_size, L1_block, L1_ways, L2_size, L2_block, L2_ways;
    cfg >> L1_size >> L1_block >> L1_ways >> L2_size >> L2_block >> L2_ways;
    cfg.close();

    Cache L1("L1", L1_size, L1_block, L1_ways);
    Cache L2("L2", L2_size, L2_block, L2_ways);
    MainMemory::init();

    ifstream memfile(argv[2]);
    if (!memfile) { cerr << "Cannot open memops file\n"; return 1; }

    string line;
    uint64_t oob_ops = 0;
    while (getline(memfile, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        auto parts = split_whitespace(line);
        if (parts.size() < 2) continue;
        char op = toupper(parts[0][0]);
        uint64_t addr = stoull(parts[1], nullptr, 0);
        if (!MainMemory::in_bounds(addr)) { cout << op << " " << parts[1] << " -> OOB\n"; oob_ops++; continue; }

        if (op == 'R') {
            string path;
            uint8_t val = L1.read_byte((uint32_t)addr, &L2, path);
            cout << "R " << parts[1] << " -> " << path << " | value=" << (unsigned)val << "\n";
        } else if (op == 'W' && parts.size() >= 3) {
            unsigned v = stoul(parts[2]);
            if (v > 255) { cout << "value must be 0..255\n"; continue; }
            L1.write_byte((uint32_t)addr, &L2, (uint8_t)v); // NOTE: signature in our Cache.cpp is write_byte(addr, Cache*, val)
            cout << "W " << parts[1] << " = " << v << " -> OK\n";
        }

        if (verbose) {
            L1.debug_print(); L2.debug_print(); MainMemory::print_state();
        }
    }

    cout << "\n=== Final Summary ===\n";
    L1.debug_print(); L2.debug_print(); MainMemory::print_state();
    cout << "OOB ops: " << oob_ops << "\n";
    return 0;
}
