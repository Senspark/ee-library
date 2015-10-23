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
void Image::tentBlur1D(cocos2d::Image* image, unsigned range) {
    tentBlur1D(image->getData(), (unsigned) image->getWidth(), (unsigned) image->getHeight(), range);
}

void Image::tentBlur1D(unsigned char* pixels, unsigned width, unsigned height, unsigned range) {
    unsigned kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    auto weights = new unsigned[kernelSize + 1]; weights[0] = 0;
    for (unsigned size = 1; size <= range; ++size) {
        weights[size] = weights[size - 1] + size;
    }
    for (unsigned size = range + 1; size <= kernelSize; ++size) {
        weights[size] = weights[size - 1] + kernelSize - size + 1;
    }
    
    constexpr unsigned Bits = 24;
    const unsigned Numerator = (1 << Bits) / weights[kernelSize];
    
    int dimension = std::max(width, height);
    auto __sumR = new unsigned[dimension + 1]; __sumR[0] = 0;
    auto __sumG = new unsigned[dimension + 1]; __sumG[0] = 0;
    auto __sumB = new unsigned[dimension + 1]; __sumB[0] = 0;
    auto buffer = new unsigned char[dimension * 4];
    
    auto newPixel = buffer;
    auto sumInR = __sumR;
    auto sumInG = __sumG;
    auto sumInB = __sumB;
    auto sumMidR = __sumR;
    auto sumMidG = __sumG;
    auto sumMidB = __sumB;
    auto sumOutR = __sumR;
    auto sumOutG = __sumG;
    auto sumOutB = __sumB;
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
    auto addPixelHori = [&] {
        *(sumInR + 1) = *sumInR + *inPixel++;
        *(sumInG + 1) = *sumInG + *inPixel++;
        *(sumInB + 1) = *sumInB + *inPixel++;
        inPixel++;
    };
    auto addPixelVert = [&] {
        *(sumInR + 1) = *sumInR + *(inPixel + 0);
        *(sumInG + 1) = *sumInG + *(inPixel + 1);
        *(sumInB + 1) = *sumInB + *(inPixel + 2);
        inPixel += width * 4;
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
    auto updatePixel = [&](unsigned hits) {
        *newPixel++ = (unsigned char) (sumR / hits);
        *newPixel++ = (unsigned char) (sumG / hits);
        *newPixel++ = (unsigned char) (sumB / hits);
        *newPixel++ = 0xFF;
    };
    auto updatePixelFullKernel = [&] {
        *newPixel++ = (unsigned char) ((sumR * Numerator) >> Bits);
        *newPixel++ = (unsigned char) ((sumG * Numerator) >> Bits);
        *newPixel++ = (unsigned char) ((sumB * Numerator) >> Bits);
        *newPixel++ = 0xFF;
    };
    
    // Horizonal pass.
    auto horiPixel = pixels;
    for (unsigned row = 0; row < height; ++row) {
        sumR = sumG = sumB = 0;
        sumInR = sumMidR = sumOutR = __sumR;
        sumInG = sumMidG = sumOutG = __sumG;
        sumInB = sumMidB = sumOutB = __sumB;
        newPixel = buffer;
        for (unsigned col = 0; col < range; ++col) {
            addPixelHori();
            shiftSumIn();
            addSum();
        }
        for (unsigned col = range; col <= range + range; ++col) {
            addPixelHori();
            shiftSumIn();
            addSum();
            removeSum();
            shiftSumMid();
            updatePixel(weights[col + 1]);
        }
        for (unsigned col = range + range + 1; col < width; ++col) {
            addPixelHori();
            shiftSumIn();
            addSum();
            removeSum();
            shiftSumMid();
            shiftSumOut();
            updatePixelFullKernel();
        }
        for (unsigned col = 0; col < range; ++col) {
            addSum();
            removeSum();
            shiftSumMid();
            shiftSumOut();
            updatePixel(weights[kernelSize - col - 1]);
        }
        std::memcpy(horiPixel, buffer, width * 4);
        horiPixel += width * 4;
    }
    
    // Vertical pass.
    auto vertPixel = pixels;
    for (unsigned col = 0; col < width; ++col) {
        sumR = sumG = sumB = 0;
        sumInR = sumMidR = sumOutR = __sumR;
        sumInG = sumMidG = sumOutG = __sumG;
        sumInB = sumMidB = sumOutB = __sumB;
        newPixel = buffer;
        inPixel = vertPixel;
        for (unsigned row = 0; row < range; ++row) {
            addPixelVert();
            shiftSumIn();
            addSum();
        }
        for (unsigned row = range; row <= range + range; ++row) {
            addPixelVert();
            shiftSumIn();
            addSum();
            removeSum();
            shiftSumMid();
            updatePixel(weights[row + 1]);
        }
        for (unsigned row = range + range + 1; row < height; ++row) {
            addPixelVert();
            shiftSumIn();
            addSum();
            removeSum();
            shiftSumMid();
            shiftSumOut();
            updatePixelFullKernel();
        }
        for (unsigned row = 0; row < range; ++row) {
            addSum();
            removeSum();
            shiftSumMid();
            shiftSumOut();
            updatePixel(weights[kernelSize - row - 1]);
        }
        for (int row = 0; row < height; ++row) {
            std::memcpy(vertPixel + row * 4 * width, buffer + row * 4, 4);
        }
        vertPixel += 4;
    }
    
    delete[] buffer;
    delete[] __sumR;
    delete[] __sumG;
    delete[] __sumB;
    delete[] weights;
}
namespace_ee_end
