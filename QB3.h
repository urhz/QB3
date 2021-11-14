#pragma once
#include <vector>
#include <cinttypes>
#include <limits>
//#include <functional>
#include <cassert>
#include <utility>
#if defined(_WIN32)
#include <intrin.h>
#endif
#include "qb3_tables.h"

// Define this to minimize the size of the code, loosing some speed for large data types
// #define QB3_OPTIMIZE_ONLY_BYTE

#if defined(__GNUC__) && defined(__x86_64__)
// Comment out if binary is to run on processors without sse4
#pragma GCC target("sse4")
#endif

namespace QB3 {
    // Masks, from 0 to 64 rung
#define M(v) (~0ull >> (64 - (v)))
// A row of 8 masks, starting with mask(n)
#define R(n) M(n), M(n + 1), M(n + 2), M(n + 3), M(n + 4), M(n + 5), M(n + 6), M(n + 7)
    const uint64_t mask[] = { 0, R(1), R(9), R(17), R(25), R(33), R(41), R(49), R(57) };
#undef R

#if defined(_WIN32_)
// rank of top set bit, result is undefined for val == 0
static size_t topbit(uint64_t val) {
    return 63 - __lzcnt64(val);
}

static size_t setbits(uint64_t val) {
    return __popcnt64(val);
}

static size_t setbits16(uint64_t val) {
    return __popcnt64(val);
}

#elif defined(__GNUC__)
static size_t topbit(uint64_t val) {
    return 63 - __builtin_clzll(val);
}

static size_t setbits(uint64_t val) {
    return __builtin_popcountll(val);
}

static size_t setbits16(uint64_t val) {
    return setbits(val);
}

#else // no builtins, portable C
static size_t topbit(uint64_t val) {
    unsigned r = 0;
    while (val >>= 1)
        r++;
    return r;
}

// My own portable byte bitcount
static inline size_t nbits(uint8_t v) {
    return ((((v - ((v >> 1) & 0x55u)) * 0x1010101u) & 0x30c00c03u) * 0x10040041u) >> 0x1cu;
}

static size_t setbits(uint64_t val) {
    return nbits(0xff & val) + nbits(0xff & (val >> 8)) + nbits(0xff & (val >> 16)) + nbits(0xff & (val >> 24))
        + nbits(0xff & (val >> 32)) + nbits(0xff & (val >> 40)) + nbits(0xff & (val >> 48)) + nbits(0xff & (val >> 56));
}

static size_t setbits16(uint64_t val) {
    return nbits(0xff & val) + nbits(0xff & (val >> 8));
}
#endif

// Looks for 1*0* in the rung bits of the input values, returns the position of last 1
// Assumes at least one of the bits is set
template<typename T>
static size_t stepleft(const T * const v, size_t rung) {
    const int B2 = 16;
    // We are looking for 1*0* pattern on the B2 bits
    uint64_t acc = ~0ull;
    // collect inverted rung bits, we are now looking for 0*1*
    for (int i = 0; i < B2; i++)
        acc = (acc << 1) | (1 & (v[i] >> rung));
    // Flip bits so the top ones are 0 and bottom ones are 1
    acc = ~acc;

    // Can call topbit since acc != 0
    if (topbit(acc) != setbits16(acc) - 1)
        return B2;
    return B2 - 1 - setbits16(acc);
}

// Undo the stepdown shift
// Looks for 1*0* in the rung bits of the input values, returns the position of first 0
// If the group was stepdown left shifted, the last rung bit has to be 0
template<typename T>
static size_t stepright(const T* const v, size_t rung) {
    const int B2 = 16;
    // We are looking for 1*0* pattern on the B2 bits
    uint64_t acc = ~0ull;
    // collect inverted rung bits, we are now looking for 0*1*
    for (int i = 0; i < B2; i++)
        acc = (acc << 1) | (1 & (v[i] >> rung));
    // Flip bits so the top ones are 0 and bottom ones are 1
    acc = ~acc;

    // Can call topbit since acc != 0
    if (topbit(acc) != setbits16(acc) - 1)
        return B2;
    return B2 - setbits16(acc);
}

// Greater common denominator
template<typename T>
T gcd(const std::vector<T>& vals) {
    if (vals.empty())
        return 0;
    auto v(vals); // Work on a copy
    auto vb(v.begin()), ve(v.end());
    for (;;) {
        sort(vb, ve);
        while (!*vb) // Skip the zeros
            if (++vb == ve)
                return 0;
        auto m(*vb); // smallest value
        if (vb + 1 == ve)
            return m; // Done
        for (auto vi(vb + 1); vi < ve; vi++)
            *vi %= m;
    }
    // Not reached
    return 0;
}

// Convert from mag-sign to absolute
template<typename T>
inline T revs(T val) {
    return (val >> 1) + (val & 1);
}

// return greatest common factor
// T is always unsigned
template<typename T>
T gcode(const std::vector<T>& vals) {

    // heap of absolute values
    std::vector<T> v;
    v.reserve(vals.size());
    for (auto val : vals) {
        // ignore the zeros
        if (val == 0) continue;
        // if a value is 1 or -1, the only common factor will be 1
        if (val < 3) return 1;
        v.push_back(revs(val));
        push_heap(v.begin(), v.end(), std::greater<T>());
    }

    if (v.empty() || v.front() < 3)
        return 1;

    do {
        pop_heap(v.begin(), v.end(), std::greater<T>());
        const T m(v.back()); // never less than 2
        v.pop_back();
        for (auto& t : v) t %= m;
        v.push_back(m); // v is never empty
        make_heap(v.begin(), v.end(), std::greater<T>());
        while (v.size() && v.front() == 0) {
            pop_heap(v.begin(), v.end(), std::greater<T>());
            v.pop_back();
        }
        if (1 == v.front())
            return 1;
    } while (v.size() > 1);
    return v.front();
}

//
// Delta and sign reorder
// All these templates work only for T unsigned, integer, 2s complement types
// Even though T is unsigned, it is considered signed, that's the whole point!
//


// Encode integers as absolute magnitude and sign, so the bit 0 is the sign.
// This encoding has the top rung always zero, regardless of sign
// To keep the range exactly the same as two's complement, the magnitude of negative values is biased down by one (no negative zero)


// Change to mag-sign without conditionals, as fast as C can make it
template<typename T>
static T mags(T v) {
    return (std::numeric_limits<T>::max() * (v >> (8 * sizeof(T) - 1))) ^ (v << 1);
}

// Undo mag-sign without conditionals, as fast as C can make it
template<typename T>
static T smag(T v) {
    return (std::numeric_limits<T>::max() * (v & 1)) ^ (v >> 1);
}

// Convert a sequence to mag-sign delta
template<typename T, size_t B2 = 16>
static T dsign(T *v, T pred) {
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
        "Only works for unsigned integral types");
    for (int i = 0; i < B2; i++) {
        pred += v[i] -= pred; // or std::swap(v[i], pred); v[i] = pred - v[i] 
        v[i] = mags(v[i]);
    }
    return pred;
}

// Reverse mag-sign and delta
template<typename T, size_t B2 = 16>
static T undsign(T *v, T pred) {
    for (int i = 0; i < B2; i++)
        v[i] = pred += smag(v[i]);
    return pred;
}

// Encoding with three codeword lenghts, used for higher rungs, not for byte data
// Yes, it's horrid, but it works. Bit fiddling!
// No conditionals, computes all three forms and chooses one by masking with the condition
// It is faster than similar code with conditions because the calculations for the three lines get interleaved
// The "(~0ull * (1 &" is to show the compiler that the multiplication is a mask operation
static std::pair<size_t, uint64_t> q3csz(uint64_t val, size_t rung) {
    uint64_t nxt = (val >> (rung - 1)) & 1;
    uint64_t top = val >> rung;
    // <size, value>
    return std::make_pair<size_t, uint64_t>(rung + top + (top | nxt),
        +((~0ull * (1 & top)) & (((val ^ (1ull << rung)) >> 2) | ((val & 0b11ull) << rung)))   // 1 x BIG     -> 00
        +((~0ull * (1 & ~(top | nxt))) & (val + (1ull << (rung - 1))))                         // 0 0 LITTLE  -> 1?
        +((~0ull * (1 & (~top & nxt))) & (val >> 1 | ((val & 1) << rung))));                   // 0 1 MIDDLE  -> 01
}

// Computed decoding, might be faster with some common factor
static std::pair<size_t, uint64_t> q3d(uint64_t acc, size_t rung) {
    uint64_t ntop = (~(acc >> (rung - 1))) & 1;
    uint64_t nnxt = (~(acc >> (rung - 2))) & 1;
    uint64_t rbit = 1ull << rung;
    return std::make_pair(rung + (ntop & 1) + (ntop & nnxt & 1),
        (1 & ~ntop) * (acc & ((rbit >> 1) - 1))
        + (1 & ntop & ~nnxt) * (((acc << 1) & (rbit - 1)) | ((acc >> rung) & 1))
        + (1 & ntop & nnxt) * (rbit + ((acc & ((rbit >> 1) - 1)) << 2) + ((acc >> rung) & 0b11))
    );
}


template <typename T = uint8_t>
std::vector<uint8_t> encode(const std::vector<T>& image,
    size_t xsize, size_t ysize, int mb = 1)
{
    constexpr size_t B = 4; // Block is 4x4 pixels
    // Unit size bit length
    constexpr size_t UBITS = sizeof(T) == 1 ? 3 : sizeof(T) == 2 ? 4 : sizeof(T) == 4 ? 5 : 6;
    // Elements in a group
    constexpr size_t B2(B * B);

    std::vector<uint8_t> result;
    result.reserve(image.size() * sizeof(T));
    Bitstream s(result);
    const size_t bands = image.size() / xsize / ysize;
    assert(image.size() == xsize * ysize * bands);
    assert(0 == xsize % B && 0 == ysize % B);

    // Running code length, start with nominal value
    std::vector<size_t> runbits(bands, sizeof(T) * 8 - 1);
    std::vector<T> prev(bands, 0u);      // Previous value, per band
    T group[B2];  // Current 2D group to encode, as array
    size_t offsets[B2];
    for (size_t i = 0; i < B2; i++)
        offsets[i] = (xsize * ylut[i] + xlut[i]) * bands;
    for (size_t y = 0; y < ysize; y += B) {
        for (size_t x = 0; x < xsize; x += B) {
            size_t loc = (y * xsize + x) * bands; // Top-left pixel address
            for (size_t c = 0; c < bands; c++) { // blocks are band interleaved

                // Collect the block for this band
                if (mb >= 0 && mb < bands && mb != c) {
                    for (size_t i = 0; i < B2; i++)
                        group[i] = image[loc + offsets[i] + c] - image[loc + offsets[i] + mb];
                }
                else {
                    for (size_t i = 0; i < B2; i++)
                        group[i] = image[loc + offsets[i] + c];
                }

                // Delta in low sign group encode
                prev[c] = dsign(group, prev[c]);
                const uint64_t maxval = *std::max_element(group, group + B2);
                const size_t rung = topbit(maxval | 1); // Force at least one bit set

                // Encode rung switch using tables, works even with no rung change
                uint64_t acc = CSW[UBITS][(rung - runbits[c]) & ((1ull << UBITS) - 1)];
                size_t abits = acc >> 12;
                acc &= 0xff; // Strip the size
                runbits[c] = rung;

                if (0 == rung) { // only 1s and 0s, rung is -1 or 0
                    acc |= maxval << abits++;
                    if (0 != maxval)
                        for (uint64_t v : group)
                            acc |= v << abits++;
                    s.push(acc, abits);
                    continue;
                }

                // If the rung bit sequence is a step down flip the last set bit, it saves one or two bits
                auto p = stepleft(group, rung);
                if (p < B2)
                    group[p] ^= static_cast<T>(1ull << rung);

                if (6 > rung) { // Encoded data fits in 64 or 128 bits
                    auto t = CRG[rung];
                    for (int i = 0; i < B2 / 2; i++) {
                        acc |= (TBLMASK & t[group[i]]) << abits;
                        abits += t[group[i]] >> 12;
                    }

                    // At rung 1 and 2 this push can be skipped, if the accum has enough space
                    if (!((rung == 1) || (rung == 2 && abits < 33))) {
                        s.push(acc, abits);
                        acc = abits = 0;
                    }

                    for (int i = B2 / 2; i < B2; i++) {
                        acc |= (TBLMASK & t[group[i]]) << abits;
                        abits += t[group[i]] >> 12;
                    }
                    s.push(acc, abits);
                    continue;
                }

                // Last part of table encoding, rung 6-7 or 6-10
                // Encoded data fits in 256 bits, 4 way interleaved
                if ((sizeof(CRG)/sizeof(*CRG)) > rung) {
                    auto t = CRG[rung];
                    size_t a[4] = { acc, 0, 0, 0 };
                    size_t asz[4] = { abits, 0, 0, 0 };
                    for (int i = 0; i < B2 / 4; i++)
                        for (int j = 0; j < 4; j++) {
                            uint16_t v = t[group[j * 4 + i]];
                            a[j] |= (TBLMASK & v) << asz[j];
                            asz[j] += v >> 12;
                        }
                    for (int j = 0; j < 4; j++)
                        s.push(a[j], asz[j]);
                    continue;
                }

                // Computed three length encoding, slower, works for rung > 1
                if (1 < sizeof(T)) { // This code vanishes in 8 bit mode
                    // Push the code switch for non-table encoding, not worth the hassle
                    s.push(acc, abits);
                    if (63 != rung) {
                        for (uint64_t val : group) {
                            auto p = q3csz(val, rung);
                            s.push(p.second, p.first);
                        }
                    }
                    else { // rung 63 may overflow 64 bits, push the second val bit explicitly
                        for (uint64_t val : group) {
                            auto p = q3csz(val, rung);
                            size_t ovf = p.first & (p.first >> 6); // overflow flag
                            s.push(p.second, p.first ^ ovf); // changes 65 in 64
                            s.push((val >> 1) & 1, ovf); // safe to call with 0 bits
                        }
                    }
                }
            }
        }
    }
    return result;
}

template<typename T = uint8_t, size_t B = 4>
std::vector<T> decode(std::vector<uint8_t>& src, size_t xsize, size_t ysize, 
    size_t bands, int mb = 1)
{
    std::vector<T> image(xsize * ysize * bands);
    Bitstream s(src);
    std::vector<T> prev(bands, 0);
    // Elements in a group
    constexpr size_t B2(B * B);
    std::vector<T> group(B2);

    // Unit size bit length
    constexpr int UBITS = sizeof(T) == 1 ? 3 : sizeof(T) == 2 ? 4 : sizeof(T) == 4 ? 5 : 6;
    auto dsw = DSW[UBITS];
    std::vector<size_t> runbits(bands, sizeof(T) * 8 - 1);

    std::vector<size_t> offsets(B2);
    for (size_t i = 0; i < B2; i++)
        offsets[i] = (xsize * ylut[i] + xlut[i]) * bands;

    uint64_t acc;
    size_t abits = 0;
    for (size_t y = 0; (y + B) <= ysize; y += B) {
        for (size_t x = 0; (x + B) <= xsize; x += B) {
            size_t loc = (y * xsize + x) * bands;
            for (int c = 0; c < bands; c++) {
                if (0) {
                    if (0 != s.get()) { // The rung change flag, triggers variable size decoding
                        uint8_t cs;
                        s.pull(cs, UBITS - 1);

                        if (cs >= (1ull << (UBITS - 2))) // Starts with 1x, short
                            cs ^= (1ull << (UBITS - 2));
                        else if (cs >= (1ull << (UBITS - 3))) // Starts with 01, middle
                            cs = static_cast<uint8_t>(s.get()) + cs * 2;
                        else { // starts with 00, long
                            uint8_t val;
                            s.pull(val, 2);
                            cs = (cs << 2) + val + (1ull << (UBITS - 1));
                        }

                        // Undo the mags operation
                        cs = smag(cs);
                        // do the positive shift if needed
                        cs += ((cs >> 7) ^ 1);

                        assert(cs != 0); // This is where the in-rung signal can be detected
                        runbits[c] = (runbits[c] + cs) & ((1ull << UBITS) - 1);
                    }
                }
                else {
                    acc = s.peek();
                    abits = 1; // Used bits
                    if (acc & 1) {
                        auto cs = dsw[(acc >> 1) & ((1ull << (UBITS + 1)) - 1)];
                        runbits[c] = (runbits[c] + cs) & ((1ull << UBITS) - 1);
                        abits = static_cast<size_t>(cs >> 12);
                    }
                }

                const size_t rung = runbits[c];
                //printf("%d\n", int(rung));
                uint64_t val;
                if (0 == rung) { // 0 or 1
                    if (0) {
                        if (0 == s.get()) // All 0s
                            fill(group.begin(), group.end(), 0);
                        else { // 0 and 1s
                            s.pull(val, group.size());
                            for (int i = 0; i < B2; i++) {
                                group[i] = val & 1;
                                val >>= 1;
                            }
                        }
                    }
                    else { // Accumulator based
                        if (0 == ((acc >> abits++) & 1))
                            fill(group.begin(), group.end(), 0);
                        else {
                            for (auto& v : group)
                                v = 1 & (acc >> abits++);
                        }
                        s.advance(abits);
                    }
                }
                else if (1 == rung) { // 2 rung nominal, could be a single bit
                    s.advance(abits);
                    for (auto& it : group)
                        if (0 != (it = static_cast<T>(s.get())))
                            if (!(it = static_cast<T>(s.get())))
                                it = static_cast<T>(s.get() | 0b10);
                }
                else { // triple length
                    s.advance(abits);
                    for (auto& it : group) {
                        if (1) {
                            s.pull(it, rung);
                            if (it > mask[rung - 1]) // Starts with 1x
                                it &= mask[rung - 1];
                            else if (it > mask[rung - 2]) // Starts with 01
                                it = static_cast<T>(s.get() + (static_cast<uint64_t>(it) << 1));
                            else { // starts with 00
                                s.pull(val, 2);
                                it = static_cast<T>((1ull << rung) + (static_cast<uint64_t>(it) << 2) + val);
                            }
                        }
                        else {
                            if (sizeof(T) != 8) {
                                auto p = q3d(s.peek(), rung);
                                it = static_cast<T>(p.second);
                                s.advance(p.first);
                            }
                            else {
                                auto acc = s.peek();
                                if (acc & (1ull << (rung - 1))) { // Starts with 1x
                                    it = acc & ((1ull << (rung - 1)) - 1);
                                    s.advance(rung);
                                }
                                else if (acc & (1ull << (rung - 2))) { // Starts with 01
                                    it = ((acc << 1) & ((1ull << rung) - 1)) | ((acc >> rung) & 1);
                                    s.advance(rung + 1);
                                }
                                else { // starts with 00, rung + 2 overflows
                                    if (sizeof(T) != 8) {
                                        it = static_cast<T>(1ull << rung) + ((acc & ((1ull << (rung - 1)) - 1)) << 2) + ((acc >> rung) & 0b11);
                                        s.advance(rung + 2);
                                    }
                                    else { // Safe for overflow due to unit size
                                        if (rung != 63) {
                                            it = static_cast<T>(1ull << rung) | ((acc & (1ull << (rung - 1)) - 1) << 2) | ((acc >> rung) & 0b11);
                                            s.advance(rung + 2);
                                        }
                                        else { // Overflow, bit 1 is not in accumulator
                                            it = static_cast<T>(1ull << rung) | ((acc & (1ull << (rung - 1)) - 1) << 2);
                                            s.advance(63);
                                            s.pull(acc, 2);
                                            it += static_cast<T>(acc);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Undo the step change
                if (rung > 0 && (0 == (group[B2 - 1] >> rung))) {
                    auto p = stepright(group.data(), rung);
                    if (p < B2)
                        group[p] ^= static_cast<T>(1) << rung;
                }
                prev[c] = undsign(group.data(), prev[c]);
                for (size_t i = 0; i < group.size(); i++)
                    image[loc + c + offsets[i]] = group[i];
            }
            for (int c = 0; c < bands; c++)
                if (mb >= 0 && mb != c)
                    for (size_t i = 0; i < group.size(); i++)
                        image[loc + c + offsets[i]] += image[loc + mb + offsets[i]];
        }
    }
    return image;
}

} // Namespace
