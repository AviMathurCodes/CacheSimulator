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

#pragma once
#include <vector>
#include <cstdint>

class MainMemory {
public:
    static constexpr uint64_t SIZE = (1ull << 32); // 4 GiB
    static constexpr size_t PAGE = 4096;           // 4 KiB
    static constexpr uint64_t NPAGES = SIZE / PAGE;

    // storage: nullptr => not yet allocated
    static std::vector<uint8_t*> pages;
    static uint64_t reads_blocks;
    static uint64_t writes_blocks;

    static void init();
    static bool in_bounds(uint64_t addr, uint64_t len = 1);
    // read/write a block starting at base; out will be resized
    static void read_block(uint32_t base, size_t bsize, std::vector<uint8_t> &out);
    static void write_block(uint32_t base, const std::vector<uint8_t> &in);

    // simple summary for printing
    static void print_state();
};
