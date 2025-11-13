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

#include "MainMemory.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>

using namespace std;

vector<uint8_t*> MainMemory::pages;
uint64_t MainMemory::reads_blocks = 0;
uint64_t MainMemory::writes_blocks = 0;

void MainMemory::init() {
    pages.assign(NPAGES, nullptr);
    reads_blocks = writes_blocks = 0;
    srand((unsigned)time(nullptr));
}

bool MainMemory::in_bounds(uint64_t addr, uint64_t len) {
    return addr < SIZE && addr + len <= SIZE;
}

static uint8_t rand_u8_simple() { return static_cast<uint8_t>(rand() % 256); }

void MainMemory::read_block(uint32_t base, size_t bsize, vector<uint8_t> &out) {
    out.assign(bsize, 0);
    for (size_t i = 0; i < bsize; ++i) {
        uint64_t a = (uint64_t)base + i;
        uint64_t pg = a / PAGE;
        size_t of = (size_t)(a % PAGE);
        if (!pages[pg]) {
            pages[pg] = new uint8_t[PAGE];
            for (size_t j = 0; j < PAGE; ++j) pages[pg][j] = rand_u8_simple();
        }
        out[i] = pages[pg][of];
    }
    reads_blocks++;
}

void MainMemory::write_block(uint32_t base, const vector<uint8_t> &in) {
    for (size_t i = 0; i < in.size(); ++i) {
        uint64_t a = (uint64_t)base + i;
        uint64_t pg = a / PAGE;
        size_t of = (size_t)(a % PAGE);
        if (!pages[pg]) {
            pages[pg] = new uint8_t[PAGE];
            for (size_t j = 0; j < PAGE; ++j) pages[pg][j] = rand_u8_simple();
        }
        pages[pg][of] = in[i];
    }
    writes_blocks++;
}

void MainMemory::print_state() {
    cout << "=== MainMemory ===\n";
    int shown = 0;
    for (size_t p = 0; p < pages.size(); ++p) {
        if (pages[p]) {
            cout << " Page " << p << " (some data)\n";
            if (++shown >= 8) { cout << "  ... (truncated)\n"; break; }
        }
    }
    cout << "Reads_blocks=" << reads_blocks << " Writes_blocks=" << writes_blocks << "\n";
}
