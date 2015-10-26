//
//  EETentBlur1D.cpp
//  EE Library
//
//  Created by enrevol on 10/22/15.
//
//

#include "EEImage.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_anonymous_begin
void internalTentBlur1D(unsigned char* pixels, unsigned width, unsigned height, unsigned range, unsigned iterations) {
    unsigned kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    constexpr unsigned Bits = 24;
    auto numerators = new unsigned[kernelSize + 1];
    for (unsigned size = 1, weight = 0; size <= kernelSize; ++size) {
        weight += (size <= range) ? size : (kernelSize - size + 1);
        numerators[size] = (1 << Bits) / weight;
    }
    
    auto prefixSumR = new unsigned[width + 1]; prefixSumR[0] = 0;
    auto prefixSumG = new unsigned[width + 1]; prefixSumG[0] = 0;
    auto prefixSumB = new unsigned[width + 1]; prefixSumB[0] = 0;
    auto buffer = new unsigned char[width * 4];
    
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
    unsigned sumR = 0;
    unsigned sumG = 0;
    unsigned sumB = 0;
    
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
    auto updatePixel = [&](unsigned size) {
        *newPixel++ = (unsigned char) ((sumR * numerators[size]) >> Bits);
        *newPixel++ = (unsigned char) ((sumG * numerators[size]) >> Bits);
        *newPixel++ = (unsigned char) ((sumB * numerators[size]) >> Bits);
        *newPixel++ = 0xFF;
    };
    
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned iteration = 0; iteration < iterations; ++iteration) {
            sumR = sumG = sumB = 0;
            sumInR = sumMidR = sumOutR = prefixSumR;
            sumInG = sumMidG = sumOutG = prefixSumG;
            sumInB = sumMidB = sumOutB = prefixSumB;
            inPixel = pixels;
            newPixel = buffer;
            for (unsigned col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
            }
            for (unsigned col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                updatePixel(col + 1);
            }
            for (unsigned col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (unsigned col = 0; col < range; ++col) {
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
    delete[] numerators;
}
namespace_anonymous_end

void Image::tentBlur1D(cocos2d::Image* image, unsigned range, unsigned iterations) {
    auto pixels = image->getData();
    auto width = (unsigned) image->getWidth();
    auto height = (unsigned) image->getHeight();
    auto buffer = new unsigned char[width * height * 4];
    auto pixelsPtr = reinterpret_cast<unsigned*>(pixels);
    auto bufferPtr = reinterpret_cast<unsigned*>(buffer);
    internalTentBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);
    internalTentBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);
    delete[] buffer;
}
namespace_ee_end
