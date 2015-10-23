//
//  EETentBlur2D.cpp
//  EE Library
//
//  Created by Hoang Le Hai on 10/23/15.
//
//

#include "EEImage.hpp"
#include "cocos2d.h"

namespace_ee_begin
void Image::tentBlur2D(cocos2d::Image* image, unsigned range) {
    tentBlur2D(image->getData(), (unsigned) image->getWidth(), (unsigned) image->getHeight(), range);
}

void Image::tentBlur2D(unsigned char* pixels, unsigned width, unsigned height, unsigned range) {
    unsigned kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    auto weights = new unsigned[kernelSize + 1]; weights[0] = 0;
    for (unsigned size = 1; size <= range; ++size) {
        weights[size] = weights[size - 1] + size;
    }
    for (unsigned size = range + 1; size <= kernelSize; ++size) {
        weights[size] = weights[size - 1] + kernelSize - size + 1;
    }
    
    constexpr unsigned MaxWidth = 1920;
    constexpr unsigned MaxHeight = 1280;
    
    static unsigned __vertSumR[MaxHeight + 1][MaxWidth];
    static unsigned __vertSumG[MaxHeight + 1][MaxWidth];
    static unsigned __vertSumB[MaxHeight + 1][MaxWidth];
    static unsigned __horiSumR[MaxWidth + 1];
    static unsigned __horiSumG[MaxWidth + 1];
    static unsigned __horiSumB[MaxWidth + 1];
    auto buffer = new unsigned char[width * height * 4];
    
    auto vertSumInR = new unsigned*[width];
    auto vertSumInG = new un
    auto vertSumInB = __vertSumB;
    auto vertSumMidR = __vertSumR;
    auto vertSumMidG = __vertSumG;
    auto vertSumMidB = __vertSumB;
    auto vertSumOutR = __vertSumR;
    auto vertSumOutG = __vertSumG;
    auto vertSumOutB = __vertSumB;
    
    auto horiSumInR = __horiSumR;
    auto horiSumInG = __horiSumG;
    auto horiSumInB = __horiSumB;
    auto horiSumMidR = __horiSumR;
    auto horiSumMidG = __horiSumG;
    auto horiSumMidB = __horiSumB;
    auto horiSumOutR = __horiSumR;
    auto horiSumOutG = __horiSumG;
    auto horiSumOutB = __horiSumB;
    
    auto pixelIn = pixels;
    
    auto shiftVertSumIn = [&] {
        ++vertSumInR;
        ++vertSumInG;
        ++vertSumInB;
    };
    auto shiftVertSumMid = [&] {
        ++vertSumMidR;
        ++vertSumMidG;
        ++vertSumMidB;
    };
    auto shiftVertSumOut = [&] {
        ++vertSumOutR;
        ++vertSumOutG;
        ++vertSumOutB;
    };
    auto shiftHoriSumIn = [&] {
        ++horiSumInR;
        ++horiSumInG;
        ++horiSumInB;
    };
    auto shiftHoriSumMid = [&] {
        ++horiSumMidR;
        ++horiSumMidG;
        ++horiSumMidB;
    };
    auto shiftHoriSumOut = [&] {
        ++horiSumOutR;
        ++horiSumOutG;
        ++horiSumOutB;
    };
    
    for (unsigned row = 0; row < range; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            shiftVertSumIn();
            *(vertSumInR + 1) = *vertSumInR + *pixelIn++;
            *(vertSumInG + 1) = *vertSumInR + *pixelIn++;
            *(vertSumInB + 1) = *vertSumInR + *pixelIn++;
            pixelIn++;
        }
    }
    for (unsigned row = range; row <= range + range; ++row) {
        for (unsigned col = 0; col < range; ++col) {
            
        }
        for (unsigned col = range; col <= range + range; ++col) {
            
        }
    }
    for (unsigned row = range + range + 1; row < height; ++row) {
        
    }
    std::memcpy(pixels, buffer, width * height * 4);
    
    delete buffer;
}
namespace_ee_end