//
//  EEShader.hpp
//  ee-library
//
//  Created by Zinge on 6/18/16.
//
//

#ifndef EE_LIBRARY_SHADER_UTILS_HPP
#define EE_LIBRARY_SHADER_UTILS_HPP

#include "EECocos2dxFwd.hpp"

namespace ee {
/// Creates a shear matrix.
/// https://en.wikipedia.org/wiki/Shear_matrix
cocos2d::Mat4 createShearZMatrix(float dx, float dy);

void transformRGB(const cocos2d::Mat4& mat, float red, float green, float blue,
                  float& transformedRed, float& transformedGreen,
                  float& transformedBlue);

/// | c 0 0 0 |
/// | 0 c 0 0 |
/// | 0 0 c 0 |
/// | t t t 1 |
/// t = (1 - c) / 2.
cocos2d::Mat4 createContrastMatrix(float contrast);

/// | r 0 0 0 |
/// | 0 g 0 0 |
/// | 0 0 b 0 |
/// | x y z 1 |
/// x = (1 - r) / 2
/// y = (1 - g) / 2
/// z = (1 - b) / 2
cocos2d::Mat4 createContrastMatrix(float red, float green, float blue);

/// | r + s   s       s       0
/// | g       g + s   s       0
/// | b       b       b + s   0
/// | 0       0       0       1
/// r = (1 - s) * 0.3086
/// g = (1 - s) * 0.6094
/// b = (1 - s) * 0.0820
cocos2d::Mat4 createSaturationMatrix(float saturation);

/// | 1 0 0 0 |
/// | 0 1 0 0 |
/// | 0 0 1 0 |
/// | b b b 1 |
cocos2d::Mat4 createBrightnessMatrix(float brightness);

/// | 1 0 0 0 |
/// | 0 1 0 0 |
/// | 0 0 1 0 |
/// | r g b 1 |
cocos2d::Mat4 createBrightnessMatrix(float red, float green, float blue);

cocos2d::Mat4 createHueMatrix(float degree);

cocos2d::GLProgramState* createHsvProgramState();

cocos2d::GLProgram* createHorizontalBlurProgram(float width, int blurRadius,
                                                bool useLinearSampling,
                                                float sigma);

cocos2d::GLProgram* createVerticalBlurProgram(float height, int blurRadius,
                                              bool useLinearSampling,
                                              float sigma);

class ShaderUtils {
public:
    static ShaderUtils* getInstance();

private:
    ShaderUtils();
    ~ShaderUtils();

    static cocos2d::GLProgram*
    createBlurProgram(bool isVertical, float dimension, int blurRadius,
                      bool useLinearSampling, float sigma);

    static std::string createBlurVertexShader(bool isVertical, float dimension,
                                              int blurRadius,
                                              bool useLinearSampling,
                                              float sigma);

    static std::string createBlurFragmentShader(int blurRadius,
                                                bool useLinearSampling,
                                                float sigma);

    static float gaussianFunction(float x, float sigma);
};
} // namespace ee

#endif /* EE_LIBRARY_SHADER_UTILS_HPP */
