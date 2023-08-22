/*
Copyright 2020-2023 Esri
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Content: Bit streams, in low endian format

Contributors:  Lucian Plesea
*/

#pragma once
#include <cinttypes>
#include <cassert>
#include <type_traits>
#include <limits>

// Input bitstream, doesn't go past size
class iBits {
public:
    iBits(const uint8_t* data, size_t size) : v(data), len(size * 8), bitp(0) {}

    // informational
    size_t avail() const { return len - bitp; }
    bool empty() const { return avail() == 0; }
    // read position in bits
    size_t position() const { return bitp; }

    // Single bit fetch
    uint64_t get() {
        if (empty()) return 0; // Don't go past the end
        uint64_t val = static_cast<uint64_t>((v[bitp / 8] >> (bitp % 8)) & 1);
        bitp++;
        return val;
    }

    // Advance read position by d bits
    void advance(size_t d) {
        bitp = (bitp + d < len) ? (bitp + d) : len;
    }

    // Get 64bits without changing the state
    uint64_t peek() const {
        if (avail() >= 64)
            return (v[bitp / 8] >> (bitp % 8)) |
            (*reinterpret_cast<const uint64_t*>(v + ((bitp + 7) / 8)) << ((8 - bitp) % 8));
        if (empty())
            return 0;
        uint64_t val = v[bitp / 8] >> (bitp % 8);
        // (bitp + bits) is byte aligned, we need data from 7 or 8 more bytes
        for (size_t bits = 8 - (bitp % 8); bits < 64 && bitp + bits < len; bits += 8)
            val |= static_cast<uint64_t>(v[(bitp + bits) / 8]) << bits;
        return val;
    }

    // Not very efficient for small number of bits
    uint64_t pull(size_t bits = 1) {
        assert(bits && bits <= 64 && !empty());
        uint64_t val = peek() & (~0ull >> (64 - bits));
        advance(bits);
        return val;
    }

private:
    const uint8_t* v;
    // In bits
    const size_t len; // in bits, multiple of 8
    size_t bitp; // read position
};

// Output bitstream, doesn't check the output buffer size
class oBits {
public:
    oBits(uint8_t * data) : v(data), bitp(0) {}

    // Do not call with val having bits above "nbits" set, the results will be corrupt
    template<typename T>
    void push(T val, size_t nbits) {
        static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
            "Only works with unsigned integral types");
        assert(nbits < 65);
        size_t used = 0;
        if (bitp % 8 != 0) { // Partial byte at the end
            v[bitp / 8] |= static_cast<uint8_t>(val << (bitp % 8));
            used = 8ull - (bitp % 8);
        }
        for (; nbits > used; used += 8)
            v[(bitp + used) / 8] = static_cast<uint8_t>(val >> used);
        bitp += nbits;
    }

    // Number of bits written
    size_t position() const {
        return bitp;
    }

    // Round position to byte boundary
    size_t tobyte() {
        bitp = (bitp + 7) & ~0x7;
        return bitp >> 3; // In bytes
    }

private:
    uint8_t *v;
    size_t bitp; // write position
};
