//
//  EESpriteWithHSV.cpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#include "EESpriteWithHsv.hpp"
#include "EEShader.hpp"

#include <cocos2d.h>

namespace ee {
SpriteWithHsv* SpriteWithHsv::createImpl(const Initializer& initializer) {
    auto result = new (std::nothrow) SpriteWithHsv();
    if (result != nullptr && initializer(result)) {
        result->autorelease();
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

SpriteWithHsv* SpriteWithHsv::create() {
    return createImpl(std::bind(&SpriteWithHsv::init, std::placeholders::_1));
}

SpriteWithHsv* SpriteWithHsv::create(const std::string& filename) {
    return createImpl([&](SpriteWithHsv* instance) {
        return instance->initWithFile(filename);
    });
}

SpriteWithHsv* SpriteWithHsv::create(const std::string& filename,
                                     const cocos2d::Rect& rect) {
    return createImpl([&](SpriteWithHsv* instance) {
        return instance->initWithFile(filename, rect);
    });
}

SpriteWithHsv* SpriteWithHsv::createWithTexture(cocos2d::Texture2D* texture) {
    return createImpl([&](SpriteWithHsv* instance) {
        return instance->initWithTexture(texture);
    });
}

SpriteWithHsv* SpriteWithHsv::createWithTexture(cocos2d::Texture2D* texture,
                                                const cocos2d::Rect& rect,
                                                bool rotated) {
    return createImpl([&](SpriteWithHsv* instance) {
        return instance->initWithTexture(texture, rect, rotated);
    });
}

SpriteWithHsv*
SpriteWithHsv::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame) {
    return createImpl(std::bind(&SpriteWithHsv::initWithSpriteFrame,
                                std::placeholders::_1, spriteFrame));
}

SpriteWithHsv*
SpriteWithHsv::createWithSpriteFrameName(const std::string& spriteFrameName) {
    return createImpl(std::bind(&SpriteWithHsv::initWithSpriteFrameName,
                                std::placeholders::_1, spriteFrameName));
}

bool SpriteWithHsv::initWithTexture(cocos2d::Texture2D* texture,
                                    const cocos2d::Rect& rect, bool rotated) {
    if (not Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }
    initShader();
    return true;
}

void SpriteWithHsv::initShader() {
    auto prog = Shader::createHsvProgram();
    setGLProgram(prog);
    getGLProgramState()->setUniformCallback(
        "u_hsv", [this](cocos2d::GLProgram* p, cocos2d::Uniform* u) {
            p->setUniformLocationWithMatrix4fv(u->location, getHsvMatrix().m,
                                               1);
        });
}

void SpriteWithHsv::draw(cocos2d::Renderer* renderer,
                         const cocos2d::Mat4& transform, std::uint32_t flags) {
    updateMatrix();
    Sprite::draw(renderer, transform, flags);
}
} // namespace ee
