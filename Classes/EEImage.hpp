//
//  EEImage.hpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#ifndef EE_LIBRARY_IMAGE_HPP
#define EE_LIBRARY_IMAGE_HPP

#include "EEMacro.hpp"
#include "EEForward.hpp"

namespace_ee_begin
class Image {
public:
    /**
     * Box blur.
     * https://en.wikipedia.org/wiki/Box_blur
     */
    static void boxBlur2D(cocos2d::Image* image, unsigned range);
    
    /**
     * Box blur with two separate passes: a horizontal pass and a vertical pass.
     */
    static void boxBlur1D(cocos2d::Image* image, unsigned range, unsigned iterations = 1);
    
    /**
     * Tent blur.
     * Equal to two iterations of box blur.
     */
    static void tentBlur1D(cocos2d::Image* image, unsigned range, unsigned iterations = 1);
    
    /**
     * Cache oblivious matrix transposition.
     */
    static void transpose(unsigned* src, unsigned* dst, unsigned srcWidth, unsigned srcHeight);
};
namespace_ee_end

#endif /* EE_LIBRARY_IMAGE_HPP */
