//
//  EEBlurBackground.cpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

#include "EEBlurBackground.hpp"
#include "EEShader.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
NS_ANONYMOUS_BEGIN

NS_ANONYMOUS_END
bool BlurBackground::init() {
    if (Node::init() == false) {
        return false;
    }

    renderScale_ = 1.0f;
    initRenderers();

    return true;
}

void BlurBackground::initRenderers() {
    CC_ASSERT(horizontalRenderer_ == nullptr);
    CC_ASSERT(verticalRenderer_ == nullptr);

    auto&& winSize = _director->getWinSize();
    setContentSize(winSize);

    auto renderSize = winSize * renderScale_;

    int width = static_cast<int>(std::ceil(renderSize.width));
    int height = static_cast<int>(std::ceil(renderSize.height));

    horizontalRenderer_ = cocos2d::RenderTexture::create(
        width, height, cocos2d::Texture2D::PixelFormat::RGBA8888,
        GL_DEPTH24_STENCIL8);

    verticalRenderer_ = cocos2d::RenderTexture::create(
        width, height, cocos2d::Texture2D::PixelFormat::RGBA8888,
        GL_DEPTH24_STENCIL8);

    horizontalRenderer_->getSprite()->setGLProgram(
        Shader::createHorizontalBlurVertexShader());
    horizontalRenderer_->getSprite()->getGLProgramState()->setUniformFloat(
        "u_dimension", width);

    verticalRenderer_->getSprite()->setGLProgram(
        Shader::createVerticalBlurVertexShader());
    verticalRenderer_->getSprite()->getGLProgramState()->setUniformFloat(
        "u_dimension", height);

    horizontalRenderer_->getSprite()->setAnchorPoint(
        cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    verticalRenderer_->getSprite()->setAnchorPoint(
        cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);

    horizontalRenderer_->setVisible(false);
    verticalRenderer_->setScale(1.0f / renderScale_);

    addChild(horizontalRenderer_, -1);
    addChild(verticalRenderer_, -1);
}

void BlurBackground::resetRenderers() {
    CC_ASSERT(horizontalRenderer_ != nullptr);
    CC_ASSERT(verticalRenderer_ != nullptr);

    horizontalRenderer_->removeFromParent();
    verticalRenderer_->removeFromParent();

    horizontalRenderer_ = nullptr;
    verticalRenderer_ = nullptr;
}

void BlurBackground::setRenderScale(float scale) {
    if (renderScale_ == scale) {
        return;
    }
    renderScale_ = scale;

    resetRenderers();
    initRenderers();
}

void BlurBackground::visit(cocos2d::Renderer* renderer,
                           const cocos2d::Mat4& parentTransforms,
                           std::uint32_t parentFlags) {
    if (isVisible()) {
        setVisible(false);
        horizontalRenderer_->beginWithClear(0, 0, 0, 0);

        auto scene = _director->getRunningScene();

        auto additionalTransform = cocos2d::Mat4::IDENTITY;
        additionalTransform.scale(renderScale_, renderScale_, 1.0f);

        scene->setAdditionalTransform(&additionalTransform);
        scene->visit();
        scene->setAdditionalTransform(nullptr);

        horizontalRenderer_->end();
        setVisible(true);

        verticalRenderer_->beginWithClear(0, 0, 0, 0);
        horizontalRenderer_->getSprite()->visit();
        verticalRenderer_->end();
    }

    Node::visit(renderer, parentTransforms, parentFlags);
}
NS_EE_END