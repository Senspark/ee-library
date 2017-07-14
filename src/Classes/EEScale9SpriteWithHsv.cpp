//
//  EEScale9SpriteWithHSV.cpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#include "EEScale9SpriteWithHsv.hpp"
#include "EEShader.hpp"

#include <cocos2d.h>

namespace ee {
Scale9SpriteWithHsv*
Scale9SpriteWithHsv::createImpl(const Initializer& initializer) {
    auto result = new (std::nothrow) Self();
    if (result != nullptr && initializer(result)) {
        result->autorelease();
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

Scale9SpriteWithHsv* Scale9SpriteWithHsv::create() {
    return createImpl([](Self* instance) { return instance->init(); });
}

Scale9SpriteWithHsv* Scale9SpriteWithHsv::create(const std::string& filename) {
    return createImpl(
        [&](Self* instance) { return instance->initWithFile(filename); });
}

Scale9SpriteWithHsv* Scale9SpriteWithHsv::create(const std::string& filename,
                                                 const cocos2d::Rect& rect) {
    return createImpl(
        [&](Self* instance) { return instance->initWithFile(filename, rect); });
}

Scale9SpriteWithHsv*
Scale9SpriteWithHsv::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame) {
    return createImpl([&](Self* instance) {
        return instance->initWithSpriteFrame(spriteFrame);
    });
}

Scale9SpriteWithHsv* Scale9SpriteWithHsv::createWithSpriteFrameName(
    const std::string& spriteFrameName) {
    return createImpl([&](Self* instance) {
        return instance->initWithSpriteFrameName(spriteFrameName);
    });
}

bool Scale9SpriteWithHsv::init() {
    if (not Super::init()) {
        return false;
    }
#if COCOS2D_VERSION >= 0x00031400
    customState_ = createState();
    setGLProgramState(customState_);
#endif // COCOS2D_VERSION >= 0x00031400
    return true;
}

void Scale9SpriteWithHsv::setGLProgramState(
    cocos2d::GLProgramState* glProgramState) {
#if COCOS2D_VERSION >= 0x00031400
    // cocos2d::ui::Scale9Sprite::setState is not virtual.
    if (glProgramState != customState_) {
        return;
    }
#endif // COCOS2D_VERSION >= 0x00031400
    Super::setGLProgramState(glProgramState);
}

bool Scale9SpriteWithHsv::updateWithSprite(cocos2d::Sprite* sprite,
                                           const cocos2d::Rect& textureRect,
                                           bool rotated,
                                           const cocos2d::Vec2& offset,
                                           const cocos2d::Size& originalSize,
                                           const cocos2d::Rect& capInsets) {
    if (not Super::updateWithSprite(sprite, textureRect, rotated, offset,
                                    originalSize, capInsets)) {
        return false;
    }
#if COCOS2D_VERSION < 0x00031400
    _scale9Image->setGLProgramState(createState());
#endif // COCOS2D_VERSION < 0x00031400
    return true;
}

void Scale9SpriteWithHsv::draw(cocos2d::Renderer* renderer,
                               const cocos2d::Mat4& transform,
                               std::uint32_t flags) {
    updateMatrix();
    Super::draw(renderer, transform, flags);
}

cocos2d::GLProgramState* Scale9SpriteWithHsv::createState() {
    auto prog = Shader::getInstance()->createHsvProgram();
    auto state = cocos2d::GLProgramState::create(prog);
    state->setUniformCallback("u_hsv", [this](cocos2d::GLProgram* p,
                                              cocos2d::Uniform* u) {
        p->setUniformLocationWithMatrix4fv(u->location, getHsvMatrix().m, 1);
    });
    return state;
}
} // namespace ee
