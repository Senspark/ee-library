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

namespace_ee_begin
namespace_anonymous_begin
void internalBoxBlur2D(unsigned char* pixels, unsigned width, unsigned height, unsigned range) {
    unsigned kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    constexpr unsigned Bits = 24;
    const unsigned Numerator = (1 << Bits) / (kernelSize * kernelSize);
    
    auto inPixel = pixels;
    auto outPixel = pixels;
    
    auto vertPrefixSumR = new unsigned[width]();
    auto vertPrefixSumG = new unsigned[width]();
    auto vertPrefixSumB = new unsigned[width]();
    auto horiPrefixSumR = new unsigned[width + 1]; horiPrefixSumR[0] = 0;
    auto horiPrefixSumG = new unsigned[width + 1]; horiPrefixSumG[0] = 0;
    auto horiPrefixSumB = new unsigned[width + 1]; horiPrefixSumB[0] = 0;
    auto buffer = new unsigned char[height * width * 4];
    
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
    auto updatePixel = [&](unsigned hits) {
        *newPixel++ = (unsigned char) ((*horiSumInR - *horiSumOutR) / hits);
        *newPixel++ = (unsigned char) ((*horiSumInG - *horiSumOutG) / hits);
        *newPixel++ = (unsigned char) ((*horiSumInB - *horiSumOutB) / hits);
        *newPixel++ = 0xFF;
    };
    auto updatePixelFullKernel = [&] {
        *newPixel++ = (unsigned char) (((*horiSumInR - *horiSumOutR) * Numerator) >> Bits);
        *newPixel++ = (unsigned char) (((*horiSumInG - *horiSumOutG) * Numerator) >> Bits);
        *newPixel++ = (unsigned char) (((*horiSumInB - *horiSumOutB) * Numerator) >> Bits);
        *newPixel++ = 0xFF;
    };
    for (unsigned row = 0; row < range; ++row) {
        resetVertSum();
        for (unsigned col = 0; col < width; ++col) {
            addPixel();
            shiftVertSum();
        }
    }
    for (unsigned row = range; row <= range + range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (unsigned col = 0; col < range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (unsigned col = range; col <= range + range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((row + 1) * (col + 1));
        }
        for (unsigned col = range + range + 1; col < width; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((row + 1) * kernelSize);
        }
        for (unsigned col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel((row + 1) * (kernelSize - col - 1));
        }
    }
    for (unsigned row = range + range + 1; row < height; ++row) {
        resetVertSum();
        resetHoriSum();
        for (unsigned col = 0; col < range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (unsigned col = range; col <= range + range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel(kernelSize * (col + 1));
        }
        for (unsigned col = range + range + 1; col < width; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixelFullKernel();
        }
        for (unsigned col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel(kernelSize * (kernelSize - col - 1));
        }
    }
    for (unsigned row = 0; row < range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (unsigned col = 0; col < range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (unsigned col = range; col <= range + range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((kernelSize - row - 1) * (col + 1));
        }
        for (unsigned col = range + range + 1; col < width; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((kernelSize - row - 1) * kernelSize);
        }
        for (unsigned col = 0; col < range; ++col) {
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
namespace_anonymous_end

void boxBlur2D(cocos2d::Image* image, unsigned range) {
    internalBoxBlur2D(image->getData(), (unsigned) image->getWidth(), (unsigned) image->getHeight(), range);
}
namespace_ee_end
