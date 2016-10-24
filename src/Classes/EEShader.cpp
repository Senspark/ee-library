//
//  EEShader.cpp
//  ee-library
//
//  Created by Zinge on 6/18/16.
//
//

#include <iomanip>

#include "EEShader.hpp"
#include "EEUtils.hpp"

#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

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

cocos2d::GLProgram* Shader::createHsvProgram() {
#include "EEHSVShader.vert.hpp"
#include "EEHSVShader.frag.hpp"

    constexpr auto ProgramKey = "ee_hsv_program";

    auto cache = cocos2d::GLProgramCache::getInstance();
    auto p = cache->getGLProgram(ProgramKey);
    if (p == nullptr) {
        p = cocos2d::GLProgram::createWithByteArrays(ee_hsv_shader_vert,
                                                     ee_hsv_shader_frag);
        cache->addGLProgram(p, ProgramKey);
    }

    CC_ASSERT(p != nullptr);
    return p;
}

cocos2d::GLProgram* Shader::createHorizontalBlurProgram(float width,
                                                        int blurRadius,
                                                        bool useLinearSampling,
                                                        float sigma) {
    auto cache = cocos2d::GLProgramCache::getInstance();
    auto key = toString("ee_horizontal_blur_", width, "_", blurRadius, "_",
                        useLinearSampling, "_", sigma);
    auto program = cache->getGLProgram(key);
    if (program == nullptr) {
        program = createBlurProgram(false, width, blurRadius, useLinearSampling,
                                    sigma);
        cache->addGLProgram(program, key);
    }
    return program;
}

cocos2d::GLProgram* Shader::createVerticalBlurProgram(float height,
                                                      int blurRadius,
                                                      bool useLinearSampling,
                                                      float sigma) {
    auto cache = cocos2d::GLProgramCache::getInstance();
    auto key = toString("ee_vertical_blur", height, "_", blurRadius, "_",
                        useLinearSampling, "_", sigma);
    auto program = cache->getGLProgram(key);
    if (program == nullptr) {
        program = createBlurProgram(true, height, blurRadius, useLinearSampling,
                                    sigma);
        cache->addGLProgram(program, key);
    }
    return program;
}

cocos2d::GLProgram* Shader::createBlurProgram(bool isVertical, float dimension,
                                              int blurRadius,
                                              bool useLinearSampling,
                                              float sigma) {
    auto vertexShader = createBlurVertexShader(
        isVertical, dimension, blurRadius, useLinearSampling, sigma);

    auto fragmentShader =
        createBlurFragmentShader(blurRadius, useLinearSampling, sigma);

    return cocos2d::GLProgram::createWithByteArrays(vertexShader.c_str(),
                                                    fragmentShader.c_str());
}

std::string Shader::createBlurVertexShader(bool isVertical, float dimension,
                                           int blurRadius,
                                           bool useLinearSampling,
                                           float sigma) {
    auto size = static_cast<std::size_t>(blurRadius + 1);

    std::vector<float> offsets;

    if (useLinearSampling && (blurRadius % 2 == 0)) {
        std::vector<float> weights(size);
        float sum = 0;
        for (std::size_t i = 0; i < size; ++i) {
            weights.at(i) = gaussianFunction(i, sigma);
            sum += weights.at(i);
            if (i > 0) {
                sum += weights.at(i);
            }
        }
        for (auto& w : weights) {
            w /= sum;
        }

        offsets.reserve(size / 2);
        for (std::size_t i = 1; i < size; i += 2) {
            offsets.push_back(
                (i * weights.at(i) + (i + 1) * weights.at(i + 1)) /
                (weights.at(i) + weights.at(i + 1)));
        }
        CC_ASSERT(offsets.size() == size / 2);
    } else {
        offsets.reserve(size - 1);
        for (std::size_t i = 1; i < size; ++i) {
            offsets.push_back(i);
        }
    }

    auto blurTexCoords = offsets.size() * 2;

    std::string result;
    result += "#ifdef GL_ES\n"
              "precision mediump float;\n"
              "#endif\n"

              "attribute vec4 a_position;\n"
              "attribute vec2 a_texCoord;\n"
              "attribute vec4 a_color;\n"

              "#ifdef GL_ES\n"
              "varying lowp vec4 v_fragmentColor;\n"
              "varying mediump vec2 v_texCoord;\n";

    result += toString("varying mediump vec2 v_blurTexCoords[", blurTexCoords,
                       "];\n");

    result += "#else\n"
              "varying vec4 v_fragmentColor;\n"
              "varying vec2 v_texCoord;\n";

    result += toString("varying vec2 v_blurTexCoords[", blurTexCoords, "];\n");

    result += "#endif\n"

              "void main() {\n"
              "    gl_Position = CC_PMatrix * a_position;\n"
              "    v_fragmentColor = a_color;\n"
              "    v_texCoord = a_texCoord;\n";

    std::size_t vertexIndex = 0;
    for (auto i = offsets.size() - 1; ~i; --i) {
        auto offset = toString(std::fixed, std::setprecision(10),
                               -offsets.at(i), " / ", dimension);
        result +=
            toString("    v_blurTexCoords[", vertexIndex++,
                     "] = v_texCoord + vec2(", (isVertical ? "0.0" : offset),
                     ", ", (isVertical ? offset : "0.0"), ");\n");
    }

    for (std::size_t i = 0; i < offsets.size(); ++i) {
        auto offset = toString(std::fixed, std::setprecision(10), offsets.at(i),
                               " / ", dimension);
        result +=
            toString("    v_blurTexCoords[", vertexIndex++,
                     "] = v_texCoord + vec2(", (isVertical ? "0.0" : offset),
                     ", ", (isVertical ? offset : "0.0"), ");\n");
    }

    result += "}";

    LOG_FUNC_FORMAT(
        "vertical = %s radius = %d sampling = %s sigma = %f frag = %s",
        isVertical ? "true" : "false", blurRadius,
        useLinearSampling ? "true" : "false", sigma, result.c_str());

    return result;
}

std::string Shader::createBlurFragmentShader(int blurRadius,
                                             bool useLinearSampling,
                                             float sigma) {
    auto size = static_cast<std::size_t>(blurRadius + 1);

    std::vector<float> weights(size);
    float sum = 0;
    for (std::size_t i = 0; i < size; ++i) {
        weights.at(i) = gaussianFunction(i, sigma);
        sum += weights.at(i);
        if (i > 0) {
            sum += weights.at(i);
        }
    }
    for (auto& w : weights) {
        w /= sum;
    }

    if (useLinearSampling && (blurRadius % 2 == 0)) {
        for (std::size_t i = 1, j = 1; i < size; i += 2) {
            weights.at(j++) = weights.at(i) + weights.at(i + 1);
        }
        weights.resize((size + 1) / 2);
    }

    auto blurTexCoords = (weights.size() - 1) * 2;

    std::string result;
    result += "#ifdef GL_ES\n"
              "precision mediump float;\n"
              "#endif\n"

              "#ifdef GL_ES\n"
              "varying lowp vec4 v_fragmentColor;\n"
              "varying mediump vec2 v_texCoord;\n";

    result += toString("varying mediump vec2 v_blurTexCoords[", blurTexCoords,
                       "];\n");

    result += "#else\n"
              "varying vec4 v_fragmentColor;\n"
              "varying vec2 v_texCoord;\n";

    result += toString("varying vec2 v_blurTexCoords[", blurTexCoords, "];\n");

    result += "#endif\n"

              "void main() {\n"
              "    gl_FragColor = vec4(0.0);\n";

    std::size_t vertexIndex = 0;
    for (auto i = weights.size() - 1; i > 0; --i) {
        result += toString(
            "    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[",
            vertexIndex++, "]) * ", std::fixed, std::setprecision(10),
            weights.at(i), ";\n");
    }

    result +=
        toString("    gl_FragColor += texture2D(CC_Texture0, v_texCoord) * ",
                 std::fixed, std::setprecision(10), weights.at(0), ";\n");

    for (std::size_t i = 1; i < weights.size(); ++i) {
        result += toString(
            "    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[",
            vertexIndex++, "]) * ", std::fixed, std::setprecision(10),
            weights.at(i), ";\n");
    }

    result += "    gl_FragColor = gl_FragColor * v_fragmentColor;\n"
              "}";

    LOG_FUNC_FORMAT("radius = %d sampling = %s sigma = %f frag = %s",
                    blurRadius, useLinearSampling ? "true" : "false", sigma,
                    result.c_str());

    return result;
}

float Shader::gaussianFunction(float x, float sigma) {
    // 1/(sigma * sqrt(2 * pi) * exp(-(x/sigma)^2 / 2).
    constexpr float inv_sqrt_2pi = 0.39894228f;

    float k = x / sigma;
    return inv_sqrt_2pi / sigma * std::exp(-k * k * 0.5f);
}
NS_EE_END
