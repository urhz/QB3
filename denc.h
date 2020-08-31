#pragma once
#include <vector>
#include <cinttypes>
#include <limits>

namespace SiBi {
// Masks, from 0 to 64 bits
#define M(v) (~0ull >> (64 - (v)))
// A row of 8 masks, starting with mask(n)
#define R(n) M(n), M(n + 1), M(n + 2), M(n + 3), M(n + 4), M(n + 5), M(n + 6), M(n + 7)
    const uint64_t mask[] = {0, R(1), R(9), R(17), R(25), R(33), R(41), R(49), R(57)};
#undef R
#undef M

// rank of top set bit - 1
static size_t ilogb(uint64_t val) {
    for (int i = 0; i < 64; i++)
        if (val <= mask[i+1])
            return i;
    return ~0; // not reached
}

#define HALFMAX(T) (std::numeric_limits<T>::max() >> 1)

// Delta and sign reorder
// All these templates work only for unsigned, integral, 2s complement types
template<typename T = uint8_t>
static T dsign(std::vector<T>& v, T prev) {
    assert(std::is_integral<T>::value && std::is_unsigned<T>::value);
    for (auto& it : v) {
        prev += it -= prev;
        it = (it > HALFMAX(T)) ? ~(it << 1): (it << 1);
    }
    return prev;
}

// Reverse sign reorder and delta
template<typename T = uint8_t>
static T undsign(std::vector<T>& v, T prev) {
    for (auto& it : v)
        it = prev += (it & 1) ? ~(it >> 1) : (it >> 1);
    return prev;
}

// Traversal order tables, first for powers of two
static const uint8_t xp2[64] = {
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 5, 4, 5, 6, 7, 6, 7,
    4, 5, 4, 5, 6, 7, 6, 7,
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 5, 4, 5, 6, 7, 6, 7,
    4, 5, 4, 5, 6, 7, 6, 7
};

static const uint8_t yp2[64] = {
    0, 0, 1, 1, 0, 0, 1, 1,
    2, 2, 3, 3, 2, 2, 3, 3,
    0, 0, 1, 1, 0, 0, 1, 1,
    2, 2, 3, 3, 2, 2, 3, 3,
    4, 4, 5, 5, 4, 4, 5, 5,
    6, 6, 7, 7, 6, 6, 7, 7,
    4, 4, 5, 5, 4, 4, 5, 5,
    6, 6, 7, 7, 6, 6, 7, 7
};

static const uint8_t x3[9] = {
    0, 1, 0, 1, 2, 2, 0, 1, 2
};
static const uint8_t y3[9] = {
    0, 0, 1, 1, 0, 1, 2, 2, 2
};

static const uint8_t x5[25] = {
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 4, 4, 4,
    0, 1, 2, 3, 4
};

static const uint8_t y5[25] = {
    0, 0, 1, 1, 0, 0, 1, 1,
    2, 2, 3, 3, 2, 2, 3, 3,
    0, 1, 2, 3,
    4, 4, 4, 4, 4
};

static const uint8_t x6[36] = {
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 5, 4, 5, 4, 5, 4, 5,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 5, 4, 5
};

static const uint8_t y6[36] = {
    0, 0, 1, 1, 0, 0, 1, 1,
    2, 2, 3, 3, 2, 2, 3, 3,
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 5, 5, 4, 4, 5, 5,
    4, 4, 5, 5
};

static const uint8_t x7[49] = {
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 0, 1, 2, 3, 2, 3,
    4, 5, 4, 5, 6, 6,
    4, 5, 4, 5, 6, 6,
    0, 1, 0, 1, 2, 3, 2, 3,
    0, 1, 2, 3,
    4, 5, 4, 5, 6, 6,
    4, 5, 6,
};

static const uint8_t y7[49] = {
    0, 0, 1, 1, 0, 0, 1, 1,
    2, 2, 3, 3, 2, 2, 3, 3,
    0, 0, 1, 1, 0, 1,
    2, 2, 3, 3, 2, 3,
    4, 4, 5, 5, 4, 4, 5, 5,
    6, 6, 6, 6,
    4, 4, 5, 5, 4, 5,
    6, 6, 6
};

static const uint8_t* xx[9] = { xp2, xp2, xp2, x3, xp2, x5, x6, x7, xp2 };
static const uint8_t* yy[9] = { yp2, yp2, yp2, y3, yp2, y5, y6, y7, yp2 };

// Block size should be 8, for noisy images 4 is better
// 2 generates too much overhead
// 16 might work for slow varying inputs, 
// which would need the lookup tables extended
// Works for non-power of two blocks, using custom tables
// Encoding with three codeword lenghts
// Does not need the maxvalue to be encoded
// only the reference number of bits
template <typename T = uint8_t>
std::vector<uint8_t> sincode(const std::vector<T>& image,
    size_t xsize, size_t ysize, size_t bsize, int mb = 1)
{
    std::vector<uint8_t> result;
    Bitstream s(result);
    const uint8_t* xlut = xx[bsize];
    const uint8_t* ylut = yy[bsize];
    size_t bands = image.size() / xsize / ysize;
    // Nominal bit length
    constexpr uint8_t ubits = sizeof(T) == 1 ? 3 : sizeof(T) == 2 ? 4 : sizeof(T) == 4 ? 5 : 6;
    std::vector<size_t> runbits(bands, sizeof(T)*8 - 1); // Running code length, start with nominal value
    std::vector<T> prev(bands, 0u);      // Previous value per band
    std::vector<T> group(bsize * bsize); // Current 2D group to encode, as vector

    for (size_t y = 0; (y + bsize) <= ysize; y += bsize) {
        for (size_t x = 0; (x + bsize) <= xsize; x += bsize) {
            size_t loc = (y * xsize + x) * bands; // Top-left pixel address
            for (size_t c = 0; c < bands; c++) { // blocks are band interleaved
                // Collect the block for this band
                for (size_t i = 0; i < group.size(); i++)
                    group[i] = image[loc + c + (ylut[i] * xsize + xlut[i]) * bands];
                // Subtract the main group values
                if (mb >= 0 && mb != c)
                    for (size_t i = 0; i < group.size(); i++)
                        group[i] -= image[loc + mb + (ylut[i] * xsize + xlut[i]) * bands];
                // Delta with low sign group encode
                prev[c] = dsign(group, prev[c]);
                uint64_t maxval = *max_element(group.begin(), group.end());
                if (maxval < 2) {
                    // Special encoding, 000 0 for all zeros, or 000 1 followed by bit vector for 0 and 1
                    if (runbits[c] == 0)
                        s.push(maxval << 1, 2); // one zero bit prefix, same as before and the all zero flag
                    else {
                        s.push((maxval << (ubits + 1)) + 1, ubits + 2); // one 1 bit, then the 0 ubit len, then the zero flag
                        runbits[c] = 0;
                    }
                    if (1 == maxval) {
                        uint64_t val = 0;
                        for (auto it = group.crbegin(); it != group.crend(); it++)
                            val = (val << 1) + *it;
                        s.push(val, group.size());
                    }
                    continue;
                }

                if (maxval < 4) { // Doesn't always have 2 detection bits
                    if (runbits[c] == 1)
                        s.push(0u, 1); // Same, just one bit
                    else {
                        s.push(3u, ubits + 1); // change flag, + 1 as ubit len
                        runbits[c] = 1;
                    }
                    for (auto it : group)
                        if (it < 2)
                            s.push(it * 3u, 1ull + it);
                        else
                            s.push(((it & 1) << 2) | 1u, 3);
                    continue;
                }

                // Number of bits after the fist 1
                size_t bits = ilogb(maxval);
                if (bits == runbits[c])
                    s.push(0u, 1); // Same as before, just a zero bit
                else {
                    s.push(bits * 2 | 1, ubits + 1); // change bit + len on ubits
                    runbits[c] = bits;
                }

                // Three length encoding
                // The bottom bits are read as a group
                // which starts with the two detection bits
                for (uint64_t val : group) {
                    if (val <= mask[bits - 1]) { // short codewords
                        val |= 1ull << (bits - 1);
                        s.push(val, bits); // Starts with 1
                    }
                    else if (val <= mask[bits]) { // Second quarter, nominal
                        val = (val >> 1) | ((val & 1) << bits);
                        s.push(val, bits + 1); // starts with 01, rotated right one bit
                    }
                    else { // last half, least likely, long codewords
                        val &= mask[bits];
                        if (bits < 63) {
                            val = (val >> 2) | ((val & 0b11) << bits);
                            s.push(val, bits + 2); // starts with 00, rotated two bits
                        }
                        else { // bits == 63, hardly ever happens
                            s.push(val >> 2, bits);
                            s.push(val & 0b11, 2);
                        }
                    }
                }
            }
        }
    }
    return s.v;
}

template<typename T = uint8_t>
std::vector<T> unsin(std::vector<uint8_t>& src, size_t xsize, size_t ysize, 
    size_t bands, size_t bsize, int mb = 1)
{
    std::vector<T> image(xsize * ysize * bands);
    Bitstream s(src);
    std::vector<T> prev(bands, 0);
    std::vector<T> group(bsize * bsize);
    const uint8_t* xlut = xx[bsize];
    const uint8_t* ylut = yy[bsize];
    constexpr int ubits = sizeof(T) == 1 ? 3 : sizeof(T) == 2 ? 4 : sizeof(T) == 4 ? 5 : 6;
    std::vector<size_t> runbits(bands, sizeof(T)*8 - 1);

    for (size_t y = 0; (y + bsize) <= ysize; y += bsize) {
        for (size_t x = 0; (x + bsize) <= xsize; x += bsize) {
            size_t loc = (y * xsize + x) * bands;
            for (int c = 0; c < bands; c++) {
                uint64_t bits = runbits[c];
                if (0 != s.get()) { // The bits change flag
                    s.pull(runbits[c], ubits);
                    bits = runbits[c];
                }
                uint64_t val = 0;
                if (0 == bits) { // 0 or 1
                    if (0 == s.get()) // All 0s
                        fill(group.begin(), group.end(), 0);
                    else { // 0 and 1s
                        s.pull(val, group.size());
                        for (auto& it : group) {
                            it = val & 1;
                            val >>= 1;
                        }
                    }
                }
                else if (1 == bits) { // 2 bits nominal, could be a single bit
                    for (auto& it : group) {
                        if (0 != s.get()) {
                            s.pull(it);
                            if (0 == it) {
                                s.pull(it);
                                it |= 0b10;
                            }
                        }
                        else
                            it = 0;
                    }
                }
                else { // triple length
                    for (auto& it : group) {
                        s.pull(it, bits);
                        if (it > mask[bits - 1]) // Starts with 1
                            it &= mask[bits - 1];
                        else if (it > mask[bits - 2])
                            it = static_cast<T>(s.get() + (it << 1));
                        else {
                            s.pull(val, 2);
                            it = static_cast<T>((1ull << bits) + (it << 2) + val);
                        }
                    }
                }
                prev[c] = undsign(group, prev[c]);
                for (size_t i = 0; i < group.size(); i++)
                    image[loc + c + (ylut[i] * xsize + xlut[i]) * bands] = group[i];
            }
            if (mb >= 0)
                for (int c = 0; c < bands; c++)
                    if (mb != c)
                        for (size_t i = 0; i < group.size(); i++)
                            image[loc + c + (ylut[i] * xsize + xlut[i]) * bands] +=
                            image[loc + mb + (ylut[i] * xsize + xlut[i]) * bands];
        }
    }
    return image;
}
} // Namespace SiBi
