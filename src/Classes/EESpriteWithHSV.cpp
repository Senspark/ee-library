//
//  EESpriteWithHSV.cpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#include "EESpriteWithHSV.hpp"
#include "EEShader.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
NS_ANONYMOUS_BEGIN
#include "EEHSVShader.vert.hpp"
#include "EEHSVShader.frag.hpp"
NS_ANONYMOUS_END

SpriteWithHSV* SpriteWithHSV::createImpl(const Initializer& initializer) {
    auto result = new (std::nothrow) SpriteWithHSV();
    if (result != nullptr && initializer(result)) {
        result->autorelease();
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

SpriteWithHSV* SpriteWithHSV::create() {
    return createImpl(std::bind(&SpriteWithHSV::init, std::placeholders::_1));
}

SpriteWithHSV* SpriteWithHSV::create(const std::string& filename) {
    return createImpl([&](SpriteWithHSV* instance) {
        return instance->initWithFile(filename);
    });
}

SpriteWithHSV* SpriteWithHSV::create(const std::string& filename,
                                     const cocos2d::Rect& rect) {
    return createImpl([&](SpriteWithHSV* instance) {
        return instance->initWithFile(filename, rect);
    });
}

SpriteWithHSV* SpriteWithHSV::createWithTexture(cocos2d::Texture2D* texture) {
    return createImpl([&](SpriteWithHSV* instance) {
        return instance->initWithTexture(texture);
    });
}

SpriteWithHSV* SpriteWithHSV::createWithTexture(cocos2d::Texture2D* texture,
                                                const cocos2d::Rect& rect,
                                                bool rotated) {
    return createImpl([&](SpriteWithHSV* instance) {
        return instance->initWithTexture(texture, rect, rotated);
    });
}

SpriteWithHSV*
SpriteWithHSV::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame) {
    return createImpl(std::bind(&SpriteWithHSV::initWithSpriteFrame,
                                std::placeholders::_1, spriteFrame));
}

SpriteWithHSV*
SpriteWithHSV::createWithSpriteFrameName(const std::string& spriteFrameName) {
    return createImpl(std::bind(&SpriteWithHSV::initWithSpriteFrameName,
                                std::placeholders::_1, spriteFrameName));
}

bool SpriteWithHSV::initWithTexture(cocos2d::Texture2D* texture) {
    return Sprite::initWithTexture(texture);
}

bool SpriteWithHSV::initWithTexture(cocos2d::Texture2D* texture,
                                    const cocos2d::Rect& rect, bool rotated) {
    if (Sprite::initWithTexture(texture, rect, rotated) == false) {
        return false;
    }
    initShader();

    setHue(0.0f);
    setSaturation(1.0f);
    setBrightness(0.0f);
    setContrast(1.0f);

    return true;
}

void SpriteWithHSV::initShader() {
    constexpr auto ProgramKey = "ee_hsv_program";

    auto cache = cocos2d::GLProgramCache::getInstance();
    auto p = cache->getGLProgram(ProgramKey);
    if (p == nullptr) {
        p = cocos2d::GLProgram::createWithByteArrays(ee_hsv_shader_vert,
                                                     ee_hsv_shader_frag);
        //        p->bindAttribLocation(cocos2d::GLProgram::NAME, GLuint index)
        cache->addGLProgram(p, ProgramKey);
    }

    CC_ASSERT(p != nullptr);

    setGLProgram(p);
    getGLProgramState()->setUniformCallback(
        "u_hsv", CC_CALLBACK_2(SpriteWithHSV::uniformCallback, this));
}

void SpriteWithHSV::draw(cocos2d::Renderer* renderer,
                         const cocos2d::Mat4& transform, std::uint32_t flags) {
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    auto visitingCamera = cocos2d::Camera::getVisitingCamera();
    auto defaultCamera = cocos2d::Camera::getDefaultCamera();
    if (visitingCamera == defaultCamera) {
        _insideBounds = ((flags & FLAGS_TRANSFORM_DIRTY) ||
                         visitingCamera->isViewProjectionUpdated())
                            ? renderer->checkVisibility(transform, _contentSize)
                            : _insideBounds;
    } else {
        _insideBounds = renderer->checkVisibility(transform, _contentSize);
    }

    if (_insideBounds)
#endif
    {
        updateMatrix();
        _trianglesCommand.init(_globalZOrder, _texture->getName(),
                               getGLProgramState(), _blendFunc,
                               _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);
    }
}

void SpriteWithHSV::setHue(float hue) {
    if (hue == hue_) {
        return;
    }
    hue_ = hue;
    matrixDirty_ = hueMatrixDirty_ = true;
}

void SpriteWithHSV::setSaturation(float saturation) {
    if (saturation == saturation_) {
        return;
    }
    saturation_ = saturation;
    matrixDirty_ = saturationMatrixDirty_ = true;
}

void SpriteWithHSV::setBrightness(float brightness) {
    if (brightness == brightness_) {
        return;
    }
    brightness_ = brightness;
    matrixDirty_ = brightnessMatrixDirty_ = true;
}

void SpriteWithHSV::setContrast(float contrast) {
    if (contrast == contrast_) {
        return;
    }
    contrast_ = contrast;
    matrixDirty_ = contrastMatrixDirty_ = true;
}

void SpriteWithHSV::updateMatrix() {
    updateHueMatrix();
    updateSaturationMatrix();
    updateBrightnessMatrix();
    updateContrastMatrix();

    if (matrixDirty_) {
        matrixDirty_ = false;
        hsvMatrix_.setIdentity();
        cocos2d::Mat4::multiply(hueMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(saturationMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(brightnessMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(contrastMatrix_, hsvMatrix_, &hsvMatrix_);
    }
}

void SpriteWithHSV::updateHueMatrix() {
    if (hueMatrixDirty_) {
        hueMatrixDirty_ = false;
        hueMatrix_ = Shader::createHueMatrix(hue_);
    }
}

void SpriteWithHSV::updateSaturationMatrix() {
    if (saturationMatrixDirty_) {
        saturationMatrixDirty_ = false;
        saturationMatrix_ = Shader::createSaturationMatrix(saturation_);
    }
}

void SpriteWithHSV::updateBrightnessMatrix() {
    if (brightnessMatrixDirty_) {
        brightnessMatrixDirty_ = false;
        brightnessMatrix_ = Shader::createBrightnessMatrix(brightness_);
    }
}

void SpriteWithHSV::updateContrastMatrix() {
    if (contrastMatrixDirty_) {
        contrastMatrixDirty_ = false;
        contrastMatrix_ = Shader::createContrastMatrix(contrast_);
    }
}

void SpriteWithHSV::uniformCallback(cocos2d::GLProgram* p,
                                    cocos2d::Uniform* u) {
    p->setUniformLocationWithMatrix4fv(u->location, hsvMatrix_.m, 1);
}
NS_EE_END