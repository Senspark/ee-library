//
//  EEImage.cpp
//  EE Library
//
//  Created by enrevol on 10/24/15.
//
//

#include "EEImage.hpp"

namespace_ee_begin
namespace_anonymous_begin
void internalTranspose(unsigned rowBegin, unsigned rowEnd, unsigned colBegin, unsigned colEnd, unsigned* src, unsigned* dst, unsigned srcWidth, unsigned srcHeight) {
    auto deltaRow = rowEnd - rowBegin;
    auto deltaCol = colEnd - colBegin;
    if (deltaRow <= 16 && deltaCol <= 16) {
        for (unsigned col = colBegin; col < colEnd; ++col) {
            for (unsigned row = rowBegin; row < rowEnd; ++row) {
                dst[col * srcHeight + row] = src[row * srcWidth + col];
            }
        }
    } else if (deltaRow >= deltaCol) {
        internalTranspose(rowBegin, rowBegin + (deltaRow / 2), colBegin, colEnd, src, dst, srcWidth, srcHeight);
        internalTranspose(rowBegin + (deltaRow / 2), rowEnd, colBegin, colEnd, src, dst, srcWidth, srcHeight);
    } else {
        internalTranspose(rowBegin, rowEnd, colBegin, colBegin + (deltaCol / 2), src, dst, srcWidth, srcHeight);
        internalTranspose(rowBegin, rowEnd, colBegin + (deltaCol / 2), colEnd, src, dst, srcWidth, srcHeight);
    }
}
namespace_anonymous_end

void transpose(unsigned* src, unsigned* dst, unsigned srcWidth, unsigned srcHeight) {
    internalTranspose(0, srcHeight, 0, srcWidth, src, dst, srcWidth, srcHeight);
}
namespace_ee_end
