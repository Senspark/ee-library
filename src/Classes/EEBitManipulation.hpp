//
//  EEBit.hpp
//  ee-library
//
//  Created by enrevol on 3/24/16.
//
//

#ifndef EE_LIBRARY_BIT_MANIPULATION_HPP_
#define EE_LIBRARY_BIT_MANIPULATION_HPP_

/// Fast bit manipulation library.

#include <climits>
#include <cstdint>

#include "EEMacro.hpp"

NS_EE_BEGIN
/// Checks whether a number is a power of two.
///
/// @warning Undefined if n < 0.
template <class T> constexpr bool isPowerOfTwo(T n) noexcept;

/// Clears the lowest (least significant) bit in n.
///
/// Example: @c 0110110 returns @c 0110100.
template <class T> constexpr T clearLowestBit(T n) noexcept;

/// Isolates the lowest (least significant) bit in n.
///
/// Example: @c 0110110 returns @c 0000010.
template <class T> constexpr T isolateLowestBit(T n) noexcept;

/// Isolates the highest (most significant) bit in n.
///
/// Example: @c 0110110 returns @c 0100000.
template <class T> constexpr T isolateHighestBit(T n) noexcept;

/// Bit scan forward using De Bruijin algorithm.
///
/// @warning Undefined if n == 0.
template <class T> constexpr std::uint32_t bitScanForward(T n) noexcept;

/// Bit scan forward (slow) using naive algorithm.
///
/// Example: @c 01100110 returns @c 1.
template <class T> constexpr std::uint32_t bitScanForwardSlow(T n) noexcept;

/// Bit scan reverse using De Bruijin algorithm.
///
/// @warning Undefined if n == 0.
template <class T> constexpr std::uint32_t bitScanReverse(T n) noexcept;

/// Calculates the number of trailing zero bits in n.
template <class T> constexpr std::uint32_t countTrailingZero(T n) noexcept;

/// Calculates the number of leading zero bits in n.
template <class T> constexpr std::uint32_t countLeadingZero(T n) noexcept;

/// Calculates the number of one bits in n.
///
/// Example: @c 0110110 returns @c 4.
template <class T> constexpr std::uint32_t bitPopCount(T n) noexcept;

/// Calculates the number of one bits in n by using Kernighan algorithm.
///
/// Faster than @c bitPopCount if the number of one bits is small.
template <class T> constexpr std::uint32_t bitPopCountKernighan(T n) noexcept;

template <class T> constexpr bool isPowerOfTwo(T n) noexcept {
    return n && clearLowestBit(n) == 0;
}

template <class T> constexpr T clearLowestBit(T n) noexcept {
    return n & (n - 1);
}

template <class T> constexpr T isolateLowestBit(T n) noexcept {
    // Maybe also use: return n & -n.
    return n ^ clearLowestBit(n);
}

template <>
constexpr std::uint64_t
isolateHighestBit<std::uint64_t>(std::uint64_t n) noexcept {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return (n & ~(n >> 1));
}

template <class T> constexpr std::uint32_t bitScanForwardSlow(T n) noexcept {
    for (std::uint32_t i = 0;; ++i)
        if ((n >> i) & 1) {
            return i;
        }
    return 0;
}

template <>
constexpr std::uint32_t
bitScanForward<std::uint64_t>(std::uint64_t n) noexcept {
    constexpr std::uint32_t seq[] = {
        0,  47, 1,  56, 48, 27, 2,  60, 57, 49, 41, 37, 28, 16, 3,  61,
        54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4,  62,
        46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30, 9,  24, 13, 18, 8,  12, 7,  6,  5,  63};
    return seq[((n ^ (n - 1)) * 0x03f79d71b4cb0a89ULL) >> 58];
}

template <>
constexpr std::uint32_t
bitScanReverse<std::uint64_t>(std::uint64_t n) noexcept {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    constexpr std::uint32_t seq[] = {
        0,  47, 1,  56, 48, 27, 2,  60, 57, 49, 41, 37, 28, 16, 3,  61,
        54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4,  62,
        46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30, 9,  24, 13, 18, 8,  12, 7,  6,  5,  63};
    return seq[(n * 0x03f79d71b4cb0a89ULL) >> 58];
}

template <>
constexpr std::uint32_t
countTrailingZero<std::uint64_t>(std::uint64_t n) noexcept {
    return n ? bitScanForward(n) : 64;
}

template <>
constexpr std::uint32_t
countLeadingZero<std::uint64_t>(std::uint64_t n) noexcept {
    return n ? (bitScanReverse(n) ^ 63) : 64;
}

template <class T> constexpr std::uint32_t bitPopCount(T n) noexcept {
    constexpr auto k0 = (T) ~(T)0 / 3;
    constexpr auto k1 = (T) ~(T)0 / 15 * 3;
    constexpr auto k2 = (T) ~(T)0 / 255 * 15;
    constexpr auto k3 = (T) ~(T)0 / 255;
    n = n - ((n >> 1) & k0);
    n = (n & k1) + ((n >> 2) & k1);
    n = (n + (n >> 4)) & k2;
    return (T)(n * k3) >> (sizeof(T) - 1) * CHAR_BIT;
}

template <class T> constexpr std::uint32_t bitPopCountKernighan(T n) noexcept {
    std::uint32_t result = 0;
    for (; n > 0; n = clearLowestBit(n)) {
        ++result;
    }
    return result;
}
NS_EE_END

#endif /* EE_LIBRARY_BIT_MANIPULATION_HPP_ */
