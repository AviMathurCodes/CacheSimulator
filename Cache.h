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
#include <string>

struct CacheLine {
    bool valid = false;
    bool dirty = false;
    uint32_t tag = 0;
    uint64_t last_use = 0;
    std::vector<uint8_t> data;
    CacheLine() {}
    CacheLine(size_t B): data(B, 0) {}
};

class Cache {
public:
    std::string name;
    int size = 0;
    int block = 0;
    int ways = 1;
    int sets_n = 0;
    uint64_t clk = 0;
    std::vector<std::vector<CacheLine>> sets;

    // stats
    uint64_t hits = 0, misses = 0, writebacks = 0;

    Cache() {}
    Cache(const std::string &n, int c, int b, int a) { name = n; configure(c,b,a); }

    void configure(int c, int b, int a);
    uint8_t read_byte(uint32_t addr, Cache* lower, std::string &path);
    void write_byte(uint32_t addr, Cache* lower, uint8_t val);
    void debug_print() const;
    double hit_percent() const;
};
