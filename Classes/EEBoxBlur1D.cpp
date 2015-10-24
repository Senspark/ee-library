//
//  EEGausianBlur.cpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#include "EEImage.hpp"
#include "cocos2d.h"

namespace_ee_begin
namespace_anonymous_begin
void internalBoxBlur1D(unsigned char* pixels, unsigned width, unsigned height, unsigned range, unsigned iterations) {
    unsigned kernelSize = range * 2 + 1;
    
    constexpr unsigned Bits = 24;
    auto numerators = new unsigned[kernelSize + 1];
    for (unsigned size = 1; size <= kernelSize; ++size) {
        numerators[size] = (1 << Bits) / size;
    }
    
    auto buffer = new unsigned char[width * 4];
    auto sumR = new unsigned[width + 1]; sumR[0] = 0;
    auto sumG = new unsigned[width + 1]; sumG[0] = 0;
    auto sumB = new unsigned[width + 1]; sumB[0] = 0;
    
    auto sumInR = sumR;
    auto sumInG = sumG;
    auto sumInB = sumB;
    auto sumOutR = sumR;
    auto sumOutG = sumG;
    auto sumOutB = sumB;
    
    auto inPixel = pixels;
    auto newPixel = buffer;
    
    auto shiftSumIn = [&] {
        ++sumInR;
        ++sumInG;
        ++sumInB;
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
    auto updatePixel = [&](unsigned size) {
        *newPixel++ = (unsigned char) (((*sumInR - *sumOutR) * numerators[size]) >> Bits);
        *newPixel++ = (unsigned char) (((*sumInG - *sumOutG) * numerators[size]) >> Bits);
        *newPixel++ = (unsigned char) (((*sumInB - *sumOutB) * numerators[size]) >> Bits);
        *newPixel++ = 0xFF;
    };
    
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned iteration = 0; iteration < iterations; ++iteration) {
            sumInR = sumOutR = sumR;
            sumInG = sumOutG = sumG;
            sumInB = sumOutB = sumB;
            inPixel = pixels;
            newPixel = buffer;
            std::memset(buffer, 0, width * 4);
            for (unsigned col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
            }
            for (unsigned col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                updatePixel(col + 1);
            }
            for (unsigned col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();            
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (unsigned col = 0; col < range; ++col) {
                shiftSumOut();
                updatePixel(kernelSize - col - 1);
            }
        }
        std::memcpy(pixels, buffer, width * 4);
        pixels = inPixel;
    }
    
    delete[] buffer;
    delete[] sumR;
    delete[] sumG;
    delete[] sumB;
    delete[] numerators;
}
namespace_anonymous_end

void Image::boxBlur1D(cocos2d::Image* image, unsigned range, unsigned iterations) {
    auto pixels = image->getData();
    auto width = (unsigned) image->getWidth();
    auto height = (unsigned) image->getHeight();
    auto buffer = new unsigned char[width * height * 4];
    auto pixelsPtr = reinterpret_cast<unsigned*>(pixels);
    auto bufferPtr = reinterpret_cast<unsigned*>(buffer);
    internalBoxBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);
    internalBoxBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);
    delete[] buffer;
}
namespace_ee_end