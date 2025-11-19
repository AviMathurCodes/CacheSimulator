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
19 Nov 2025   Avi Mathur             Changed configurations for config.tct and memops.txt to include comments. 
*/

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>

inline std::string trim(const std::string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}
inline std::vector<std::string> split_whitespace(const std::string &s) {
    std::istringstream iss(s); std::vector<std::string> out; std::string tok;
    while (iss >> tok) out.push_back(tok);
    return out;
}
inline std::string to_hex(uint32_t x) {
    std::ostringstream ss; ss << "0x" << std::hex << std::uppercase << x; return ss.str();
}

// If '//' appears anywhere, everything from that point is considered a comment and is removed.
inline std::string remove_cpp_comment(const std::string &s) {
    size_t pos = s.find("//");
    if (pos == std::string::npos) return s;
    return s.substr(0, pos);
}


