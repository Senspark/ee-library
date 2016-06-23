//
//  EEShader.cpp
//  ee-library
//
//  Created by Zinge on 6/18/16.
//
//

#include "EEShader.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
NS_ANONYMOUS_BEGIN
constexpr const float rwgt = 0.3086f;
constexpr const float gwgt = 0.6094f;
constexpr const float bwgt = 0.0820f;
NS_ANONYMOUS_END

cocos2d::Mat4 Shader::createShearZMatrix(float dx, float dy) {
    auto mat = cocos2d::Mat4::IDENTITY;
    mat.m[2] = dx;
    mat.m[6] = dy;
    return mat;
}

void Shader::transformRGB(const cocos2d::Mat4& mat, float red, float green,
                          float blue, float& transformedRed,
                          float& transformedGreen, float& transformedBlue) {
    float r = red * mat.m[0] + green * mat.m[4] + blue * mat.m[8] + mat.m[12];
    float g = red * mat.m[1] + green * mat.m[5] + blue * mat.m[9] + mat.m[13];
    float b = red * mat.m[2] + green * mat.m[6] + blue * mat.m[10] + mat.m[14];

    transformedRed = r;
    transformedGreen = g;
    transformedBlue = b;
}

cocos2d::Mat4 Shader::createContrastMatrix(float contrast) {
    return createContrastMatrix(contrast, contrast, contrast);
}

cocos2d::Mat4 Shader::createContrastMatrix(float red, float green, float blue) {
    auto mat =
        createBrightnessMatrix((1 - red) / 2, (1 - green) / 2, (1 - blue) / 2);
    mat.m[0] = red;
    mat.m[5] = green;
    mat.m[10] = blue;
    return mat;
}

cocos2d::Mat4 Shader::createSaturationMatrix(float saturation) {
    auto mat = cocos2d::Mat4::ZERO;

    mat.m[0] = (1.0f - saturation) * rwgt + saturation;
    mat.m[1] = (1.0f - saturation) * rwgt;
    mat.m[2] = (1.0f - saturation) * rwgt;

    mat.m[4] = (1.0f - saturation) * gwgt;
    mat.m[5] = (1.0f - saturation) * gwgt + saturation;
    mat.m[6] = (1.0f - saturation) * gwgt;

    mat.m[8] = (1.0f - saturation) * bwgt;
    mat.m[9] = (1.0f - saturation) * bwgt;
    mat.m[10] = (1.0f - saturation) * bwgt + saturation;

    mat.m[15] = 1.0f;

    return mat;
}

cocos2d::Mat4 Shader::createBrightnessMatrix(float brightness) {
    return createBrightnessMatrix(brightness, brightness, brightness);
}

cocos2d::Mat4 Shader::createBrightnessMatrix(float red, float green,
                                             float blue) {
    cocos2d::Mat4 mat;
    cocos2d::Mat4::createTranslation(red, green, blue, &mat);
    return mat;
}

cocos2d::Mat4 Shader::createHueMatrix(float degree) {
    auto mat = cocos2d::Mat4::IDENTITY;

    cocos2d::Mat4 temp;

    // Rotate the grey vector into positive Z.
    // Sin = 1/sqrt(2).
    // Cos = 1/sqrt(2).
    cocos2d::Mat4::createRotationX(static_cast<float>(M_PI_4), &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Sin = -1/sqrt(3).
    // Cos = sqrt(2/3).
    cocos2d::Mat4::createRotationY(-0.615479709f, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Shear the space to make the luminance plane horizontal.
    float lx, ly, lz;
    transformRGB(mat, rwgt, gwgt, bwgt, lx, ly, lz);

    float zsx = lx / lz;
    float zsy = ly / lz;
    temp = createShearZMatrix(zsx, zsy);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Rotate the hue.
    float rad = CC_DEGREES_TO_RADIANS(degree);
    cocos2d::Mat4::createRotationZ(rad, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Unshear the space to put the luminance plane back.
    temp = createShearZMatrix(-zsx, -zsy);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Rotate the grey vector back into place.
    // Sin = 1/sqrt(3).
    // Cos = sqrt(2/3);
    cocos2d::Mat4::createRotationY(0.615479709f, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Sin = -1/sqrt(2).
    // Cos = 1/sqrt(2).
    cocos2d::Mat4::createRotationX(static_cast<float>(-M_PI_4), &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);
    
    return mat;
}
NS_EE_END