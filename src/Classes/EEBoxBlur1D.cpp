//
//  EEGausianBlur.cpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#include "EEImage.hpp"

#include <cstring>

NS_EE_BEGIN
NS_ANONYMOUS_BEGIN
void internalBoxBlur1D(std::uint8_t* pixels, std::uint_fast32_t width, std::uint_fast32_t height, std::uint_fast32_t range, std::uint_fast32_t iterations) {
    std::uint_fast32_t kernelSize = range * 2 + 1;
    CC_ASSERT(kernelSize < width && kernelSize < height);
    
    constexpr std::uint_fast32_t Bits = 24;
    std::uint_fast32_t numerators[kernelSize + 1];
    for (std::uint_fast32_t size = 1; size <= kernelSize; ++size) {
        numerators[size] = (1 << Bits) / size;
    }
    
    auto buffer = new std::uint8_t[width * 4];
    auto sumR = new std::uint_fast32_t[width + 1]; sumR[0] = 0;
    auto sumG = new std::uint_fast32_t[width + 1]; sumG[0] = 0;
    auto sumB = new std::uint_fast32_t[width + 1]; sumB[0] = 0;
    
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
    auto updatePixel = [&](std::uint_fast32_t size) {
        *newPixel++ = static_cast<std::uint8_t>(((*sumInR - *sumOutR) * numerators[size]) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>(((*sumInG - *sumOutG) * numerators[size]) >> Bits);
        *newPixel++ = static_cast<std::uint8_t>(((*sumInB - *sumOutB) * numerators[size]) >> Bits);
        *newPixel++ = 0xFF;
    };
    
    for (std::uint_fast32_t row = 0; row < height; ++row) {
        for (uint_fast32_t iteration = 0; iteration < iterations; ++iteration) {
            sumInR = sumOutR = sumR;
            sumInG = sumOutG = sumG;
            sumInB = sumOutB = sumB;
            inPixel = pixels;
            newPixel = buffer;
            for (uint_fast32_t col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
            }
            for (uint_fast32_t col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                updatePixel(col + 1);
            }
            for (uint_fast32_t col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (uint_fast32_t col = 0; col < range; ++col) {
                shiftSumOut();
                updatePixel(kernelSize - col - 1);
            }
            std::memcpy(pixels, buffer, width * 4);
        }
        pixels = inPixel;
    }
    
    delete[] buffer;
    delete[] sumR;
    delete[] sumG;
    delete[] sumB;
}
NS_ANONYMOUS_END

void boxBlur1D(cocos2d::Image* image, std::uint_fast32_t range, std::uint_fast32_t iterations) {
    auto pixels = image->getData();
    auto width = static_cast<std::uint_fast32_t>(image->getWidth());
    auto height = static_cast<std::uint_fast32_t>(image->getHeight());
    auto buffer = new std::uint8_t[width * height * 4];
    auto pixelsPtr = reinterpret_cast<std::uint32_t*>(pixels);
    auto bufferPtr = reinterpret_cast<std::uint32_t*>(buffer);
    internalBoxBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);
    internalBoxBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);
    delete[] buffer;
}
NS_EE_END