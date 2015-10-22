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
void boxBlurHorizontal(unsigned char* pixels, int width, int height, int range) {
    auto buffer = new unsigned char[width * 4];
    for (int row = 0; row < height; ++row) {
        unsigned hits = 0;
        unsigned sumR = 0;
        unsigned sumG = 0;
        unsigned sumB = 0;
        auto oldPixel = pixels;
        auto newPixel = pixels;
        auto newColor = buffer;
        for (int col = -range; col < width; ++col) {
            int oldIndex = col - range - 1;
            if (oldIndex >= 0) {
                sumR -= *oldPixel++;
                sumG -= *oldPixel++;
                sumB -= *oldPixel++;
                oldPixel++;
                hits--;
            }
            int newIndex = col + range;
            if (newIndex < width) {
                sumR += *newPixel++;
                sumG += *newPixel++;
                sumB += *newPixel++;
                newPixel++;
                hits++;
            }
            if (col >= 0) {
                *newColor++ = sumR / hits;
                *newColor++ = sumG / hits;
                *newColor++ = sumB / hits;
                *newColor++ = 255;
            }
        }
        std::memcpy(pixels, buffer, width * 4);
        pixels += width * 4;
    }
    delete[] buffer;
}

void boxBlurVertical(unsigned char* pixels, int width, int height, int range) {
    auto buffer = new unsigned char[height * 4];
    for (int col = 0; col < width; ++col) {
        unsigned hits = 0;
        unsigned sumR = 0;
        unsigned sumG = 0;
        unsigned sumB = 0;
        auto oldPixel = pixels;
        auto newPixel = pixels;
        auto newColor = buffer;
        for (int row = -range; row < height; ++row) {
            int oldIndex = row - range - 1;
            if (oldIndex >= 0) {
                sumR -= *oldPixel;
                sumG -= *(oldPixel + 1);
                sumB -= *(oldPixel + 2);
                oldPixel += 4 * width;
                hits--;
            }
            int newIndex = row + range;
            if (newIndex < height) {
                sumR += *newPixel;
                sumG += *(newPixel + 1);
                sumB += *(newPixel + 2);
                newPixel += 4 * width;
                hits++;
            }
            if (row >= 0) {
                *newColor++ = sumR / hits;
                *newColor++ = sumG / hits;
                *newColor++ = sumB / hits;
                *newColor++ = 255;
            }
        }
        for (int row = 0; row < height; ++row) {
            std::memcpy(pixels + row * 4 * width, buffer + row * 4, 4);
        }
        pixels += 4;
    }
    delete[] buffer;
}
namespace_anonymous_end

void Image::boxBlur1D(cocos2d::Image* image, unsigned range) {
    boxBlur1D(image->getData(), (unsigned) image->getWidth(), (unsigned) image->getHeight(), range);
}

/**
 * Fast implementation of box blur using sliding window algorithm with two passes.
 * https://lotsacode.wordpress.com/2010/12/08/fast-blur-box-blur-with-accumulator/
 * Complexity: O(width * height + range)
 */
void Image::boxBlur1D(unsigned char* pixels, unsigned width, unsigned height, unsigned range) {
    boxBlurHorizontal(pixels, width, height, range);
    boxBlurVertical(pixels, width, height, range);
}
namespace_ee_end