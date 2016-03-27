//
//  EEBit.hpp
//  ee-library
//
//  Created by enrevol on 3/24/16.
//
//

#ifndef EEBit_hpp
#define EEBit_hpp

#include <climits>
#include <cstdint>

#include "EEMacro.hpp"

NS_EE_BEGIN
template<class T> constexpr bool isPowerOfTwo(T n);
template<class T> constexpr T isolateLowestBit(T n);
template<class T> constexpr T clearLowestBit(T n);
template<class T> constexpr T isolateHighestBit(T n);
template<class T> constexpr std::uint32_t bitScanForward(T n);
template<class T> constexpr std::uint32_t bitScanForwardSlow(T n);
template<class T> constexpr std::uint32_t bitScanReverse(T n);
template<class T> constexpr std::uint32_t countTrailingZero(T n);
template<class T> constexpr std::uint32_t countLeadingZero(T n);
template<class T> constexpr std::uint32_t bitPopCount(T n);
template<class T> constexpr std::uint32_t bitPopCountKernighan(T n);

template<class T>
constexpr bool isPowerOfTwo(T n) {
    return n && clearLowestBit(n) == 0;
}

template<class T>
constexpr T isolateLowestBit(T n) {
    // return n & -n;
    return n ^ clearLowestBit(n);
}

template<>
constexpr std::uint64_t isolateHighestBit<std::uint64_t>(std::uint64_t n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return (n & ~(n >> 1));
}

template<class T>
constexpr T clearLowestBit(T n) {
    return n & (n - 1);
}

template<class T>
constexpr std::uint32_t bitScanForwardSlow(T n) {
    for (std::uint32_t i = 0; ; ++i)
        if ((n >> i) & 1) {
            return i;
        }
    return 0;
}

template<>
constexpr std::uint32_t bitScanForward<std::uint64_t>(std::uint64_t n) {
    constexpr std::uint32_t seq[] = {
         0, 47,  1, 56, 48, 27,  2, 60,
        57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24,
        13, 18,  8, 12,  7,  6,  5, 63
    };
    return seq[((n ^ (n - 1)) * 0x03f79d71b4cb0a89ULL) >> 58];
}

template<>
constexpr std::uint32_t bitScanReverse<std::uint64_t>(std::uint64_t n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    constexpr std::uint32_t seq[] = {
        0, 47,  1, 56, 48, 27,  2, 60,
        57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24,
        13, 18,  8, 12,  7,  6,  5, 63
    };
    return seq[(n * 0x03f79d71b4cb0a89ULL) >> 58];
}

template<>
constexpr std::uint32_t countTrailingZero<std::uint64_t>(std::uint64_t n) {
    return n ? bitScanForward(n) : 64;
}

template<>
constexpr std::uint32_t countLeadingZero<std::uint64_t>(std::uint64_t n) {
    return n ? (bitScanReverse(n) ^ 63) : 64;
}

template<class T>
constexpr std::uint32_t bitPopCount(T n) {
    constexpr auto k0 = (T)~(T) 0 / 3;
    constexpr auto k1 = (T)~(T) 0 / 15 * 3;
    constexpr auto k2 = (T)~(T) 0 / 255 * 15;
    constexpr auto k3 = (T)~(T) 0 / 255;
    n = n - ((n >> 1) & k0);
    n = (n & k1) + ((n >> 2) & k1);
    n = (n + (n >> 4)) & k2;
    return (T)(n * k3) >> (sizeof(T) - 1) * CHAR_BIT;
}

template<class T>
constexpr std::uint32_t bitPopCountKernighan(T n) {
    std::uint32_t result = 0;
    for (; n > 0; n = clearLowestBit(n)) {
        ++result;
    }
    return result;
}
NS_EE_END

#endif /* EEBit_hpp */
