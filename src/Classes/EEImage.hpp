//
//  EEImage.hpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#ifndef EE_LIBRARY_IMAGE_HPP_
#define EE_LIBRARY_IMAGE_HPP_

#include "EEMacro.hpp"
#include "EEForward.hpp"

#include <cstdint>

NS_EE_BEGIN
/**
 * Box blur.
 * https://en.wikipedia.org/wiki/Box_blur
 */
void boxBlur2D(cocos2d::Image* image, std::uint_fast32_t range);

/**
 * Box blur with two separate passes: a horizontal pass and a vertical pass.
 */
void boxBlur1D(cocos2d::Image* image, std::uint_fast32_t range, std::uint_fast32_t iterations = 1);

/**
 * Tent blur.
 * Equal to two iterations of box blur.
 */
void tentBlur1D(cocos2d::Image* image, std::uint_fast32_t range, std::uint_fast32_t iterations = 1);

/**
 * Cache oblivious matrix transposition.
 */
void transpose(std::uint32_t* src, std::uint32_t* dst, std::uint_fast32_t srcWidth, std::uint_fast32_t srcHeight);
NS_EE_END

#endif /* EE_LIBRARY_IMAGE_HPP_ */