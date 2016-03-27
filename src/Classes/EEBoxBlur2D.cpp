//
//  EEBoxBlur2D.cpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#include "EEImage.hpp"
#include "cocos2d.h"

#include <cstring>

NS_EE_BEGIN
NS_ANONYMOUS_BEGIN
void internalBoxBlur2D(std::uint8_t* pixels, std::uint_fast32_t width, std::uint_fast32_t height, std::uint_fast32_t range) {
    std::uint_fast32_t kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    constexpr std::uint_fast32_t Bits = 24;
    const std::uint_fast32_t Numerator = (1 << Bits) / (kernelSize * kernelSize);
    
    auto inPixel = pixels;
    auto outPixel = pixels;
    
    auto vertPrefixSumR = new std::uint_fast32_t[width]();
    auto vertPrefixSumG = new std::uint_fast32_t[width]();
    auto vertPrefixSumB = new std::uint_fast32_t[width]();
    auto horiPrefixSumR = new std::uint_fast32_t[width + 1]; horiPrefixSumR[0] = 0;
    auto horiPrefixSumG = new std::uint_fast32_t[width + 1]; horiPrefixSumG[0] = 0;
    auto horiPrefixSumB = new std::uint_fast32_t[width + 1]; horiPrefixSumB[0] = 0;
    auto buffer = new std::uint8_t[height * width * 4];
    
    auto newPixel = buffer;
    auto vertSumR = vertPrefixSumR;
    auto vertSumG = vertPrefixSumG;
    auto vertSumB = vertPrefixSumB;
    auto horiSumInR = horiPrefixSumR;
    auto horiSumInG = horiPrefixSumG;
    auto horiSumInB = horiPrefixSumB;
    auto horiSumOutR = horiPrefixSumR;
    auto horiSumOutG = horiPrefixSumG;
    auto horiSumOutB = horiPrefixSumB;
    auto resetVertSum = [&] {
        vertSumR = vertPrefixSumR;
        vertSumG = vertPrefixSumG;
        vertSumB = vertPrefixSumB;
    };
    auto resetHoriSum = [&] {
        horiSumInR = horiSumOutR = horiPrefixSumR;
        horiSumInG = horiSumOutG = horiPrefixSumG;
        horiSumInB = horiSumOutB = horiPrefixSumB;
    };
    auto updateHoriSum = [&] {
        *(horiSumInR + 1) = *horiSumInR + *vertSumR;
        *(horiSumInG + 1) = *horiSumInG + *vertSumG;
        *(horiSumInB + 1) = *horiSumInB + *vertSumB;
    };
    auto addPixel = [&] {
        *vertSumR += *inPixel++;
        *vertSumG += *inPixel++;
        *vertSumB += *inPixel++;
        inPixel++;
    };
    auto removePixel = [&] {
        *vertSumR -= *outPixel++;
        *vertSumG -= *outPixel++;
        *vertSumB -= *outPixel++;
        outPixel++;
    };
    auto shiftVertSum = [&] {
        ++vertSumR;
        ++vertSumG;
        ++vertSumB;
    };
    auto shiftHoriSumIn = [&] {
        ++horiSumInR;
        ++horiSumInG;
        ++horiSumInB;
    };
    auto shiftHoriSumOut = [&] {
        ++horiSumOutR;
        ++horiSumOutG;
        ++horiSumOutB;
    };
    auto updatePixel = [&](std::uint_fast32_t hits) {
        *newPixel++ = static_cast<std::uint8_t>((*horiSumInR - *horiSumOutR) / hits);
        *newPixel++ = static_cast<std::uint8_t>((*horiSumInG - *horiSumOutG) / hits);
        *newPixel++ = static_cast<std::uint8_t>((*horiSumInB - *horiSumOutB) / hits);
        *newPixel++ = 0xFF;
    };
    auto updatePixelFullKernel = [&] {
        *newPixel++ = static_cast<std::uint8_t>(((*horiSumInR - *horiSumOutR) * Numerator) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>(((*horiSumInG - *horiSumOutG) * Numerator) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>(((*horiSumInB - *horiSumOutB) * Numerator) >> Bits);
        *newPixel++ = 0xFF;
    };
    for (std::uint_fast32_t row = 0; row < range; ++row) {
        resetVertSum();
        for (std::uint_fast32_t col = 0; col < width; ++col) {
            addPixel();
            shiftVertSum();
        }
    }
    for (std::uint_fast32_t row = range; row <= range + range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (std::uint_fast32_t col = range; col <= range + range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((row + 1) * (col + 1));
        }
        for (std::uint_fast32_t col = range + range + 1; col < width; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((row + 1) * kernelSize);
        }
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel((row + 1) * (kernelSize - col - 1));
        }
    }
    for (std::uint_fast32_t row = range + range + 1; row < height; ++row) {
        resetVertSum();
        resetHoriSum();
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (std::uint_fast32_t col = range; col <= range + range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel(kernelSize * (col + 1));
        }
        for (std::uint_fast32_t col = range + range + 1; col < width; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixelFullKernel();
        }
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel(kernelSize * (kernelSize - col - 1));
        }
    }
    for (std::uint_fast32_t row = 0; row < range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (std::uint_fast32_t col = range; col <= range + range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((kernelSize - row - 1) * (col + 1));
        }
        for (std::uint_fast32_t col = range + range + 1; col < width; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((kernelSize - row - 1) * kernelSize);
        }
        for (std::uint_fast32_t col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel((kernelSize - row - 1) * (kernelSize - col - 1));
        }
    }
    std::memcpy(pixels, buffer, width * height * 4);
    
    delete[] vertPrefixSumR;
    delete[] vertPrefixSumG;
    delete[] vertPrefixSumB;
    delete[] horiPrefixSumR;
    delete[] horiPrefixSumG;
    delete[] horiPrefixSumB;
    delete[] buffer;
}
NS_ANONYMOUS_END

void boxBlur2D(cocos2d::Image* image, std::uint_fast32_t range) {
    internalBoxBlur2D(image->getData(), static_cast<std::uint_fast32_t>(image->getWidth()), static_cast<std::uint_fast32_t>(image->getHeight()), range);
}
NS_EE_END
