//
//  EETentBlur1D.cpp
//  EE Library
//
//  Created by enrevol on 10/22/15.
//
//

#include "EEImage.hpp"
#include "cocos2d.h"

#include <cstring>

namespace_ee_begin
namespace_anonymous_begin
void internalTentBlur1D(std::uint8_t* pixels, std::uint_fast32_t width, std::uint_fast32_t height, std::uint_fast32_t range, std::uint_fast32_t iterations) {
    std::uint_fast32_t kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    constexpr std::uint_fast32_t Bits = 24;
    std::uint_fast32_t numerators[kernelSize + 1];
    for (std::uint_fast32_t size = 1, weight = 0; size <= kernelSize; ++size) {
        weight += (size <= range) ? size : (kernelSize - size + 1);
        numerators[size] = (1 << Bits) / weight;
    }
    
    auto prefixSumR = new std::uint_fast32_t[width + 1]; prefixSumR[0] = 0;
    auto prefixSumG = new std::uint_fast32_t[width + 1]; prefixSumG[0] = 0;
    auto prefixSumB = new std::uint_fast32_t[width + 1]; prefixSumB[0] = 0;
    auto buffer = new std::uint8_t[width * 4];
    
    auto newPixel = buffer;
    auto sumInR = prefixSumR;
    auto sumInG = prefixSumG;
    auto sumInB = prefixSumB;
    auto sumMidR = prefixSumR;
    auto sumMidG = prefixSumG;
    auto sumMidB = prefixSumB;
    auto sumOutR = prefixSumR;
    auto sumOutG = prefixSumG;
    auto sumOutB = prefixSumB;
    std::uint_fast32_t sumR = 0;
    std::uint_fast32_t sumG = 0;
    std::uint_fast32_t sumB = 0;
    
    auto inPixel = pixels;
    
    auto shiftSumIn = [&] {
        ++sumInR;
        ++sumInG;
        ++sumInB;
    };
    auto shiftSumMid = [&] {
        ++sumMidR;
        ++sumMidG;
        ++sumMidB;
    };
    auto shiftSumOut = [&] {
        ++sumOutR;
        ++sumOutG;
        ++sumOutB;
    };
    auto addPixel = [&] {
        *(sumInR + 1) = *sumInR + *inPixel++;
        *(sumInG + 1) = *sumInG + *inPixel++;
        *(sumInB + 1) = *sumInB + *inPixel++;
        inPixel++;
    };
    auto addSum = [&] {
        sumR += *sumInR - *sumMidR;
        sumG += *sumInG - *sumMidG;
        sumB += *sumInB - *sumMidB;
    };
    auto removeSum = [&] {
        sumR -= *sumMidR - *sumOutR;
        sumG -= *sumMidG - *sumOutG;
        sumB -= *sumMidB - *sumOutB;
    };
    auto updatePixel = [&](std::uint_fast32_t size) {
        *newPixel++ = static_cast<std::uint8_t>((sumR * numerators[size]) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>((sumG * numerators[size]) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>((sumB * numerators[size]) >> Bits);
        *newPixel++ = 0xFF;
    };
    
    for (std::uint_fast32_t row = 0; row < height; ++row) {
        for (std::uint_fast32_t iteration = 0; iteration < iterations; ++iteration) {
            sumR = sumG = sumB = 0;
            sumInR = sumMidR = sumOutR = prefixSumR;
            sumInG = sumMidG = sumOutG = prefixSumG;
            sumInB = sumMidB = sumOutB = prefixSumB;
            inPixel = pixels;
            newPixel = buffer;
            for (std::uint_fast32_t col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
            }
            for (std::uint_fast32_t col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                updatePixel(col + 1);
            }
            for (std::uint_fast32_t col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (std::uint_fast32_t col = 0; col < range; ++col) {
                addSum();
                removeSum();
                shiftSumMid();
                shiftSumOut();
                updatePixel(kernelSize - col - 1);
            }
            std::memcpy(pixels, buffer, width * 4);
        }
        pixels = inPixel;
    }
    
    delete[] buffer;
    delete[] prefixSumR;
    delete[] prefixSumG;
    delete[] prefixSumB;
}
namespace_anonymous_end

void tentBlur1D(cocos2d::Image* image, std::uint_fast32_t range, std::uint_fast32_t iterations) {
    auto pixels = image->getData();
    auto width = static_cast<std::uint_fast32_t>(image->getWidth());
    auto height = static_cast<std::uint_fast32_t>(image->getHeight());
    auto buffer = new std::uint8_t[width * height * 4];
    auto pixelsPtr = reinterpret_cast<std::uint32_t*>(pixels);
    auto bufferPtr = reinterpret_cast<std::uint32_t*>(buffer);
    internalTentBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);
    internalTentBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);
    delete[] buffer;
}
namespace_ee_end
