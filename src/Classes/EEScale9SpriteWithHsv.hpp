//
//  EEScale9SpriteWithHSV.hpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#ifndef EE_LIBRARY_SCALE9SPRITE_WITH_HSV_HPP_
#define EE_LIBRARY_SCALE9SPRITE_WITH_HSV_HPP_

#include "EEMacro.hpp"
#include "EEForward.hpp"
#include "EEHsvProtocol.hpp"

#include <ui/UIScale9Sprite.h>
#include <math/Mat4.h>

NS_EE_BEGIN
class Scale9SpriteWithHsv : public cocos2d::ui::Scale9Sprite,
                            public HsvProtocol {
private:
    using Self = Scale9SpriteWithHsv;
    using Super = cocos2d::ui::Scale9Sprite;
    using Initializer = std::function<bool(Self* instance)>;

    static Self* createImpl(const Initializer& initializer);

public:
    static Self* create();

    static Self* create(const std::string& filename);

    static Self* create(const std::string& filename, const cocos2d::Rect& rect);

    static Self* createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

    static Self* createWithSpriteFrameName(const std::string& spriteFrameName);

    virtual void
    setGLProgramState(cocos2d::GLProgramState* glProgramState) override;

protected:
    virtual bool init() override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

    virtual bool updateWithSprite(cocos2d::Sprite* sprite,
                                  const cocos2d::Rect& textureRect,
                                  bool rotated, const cocos2d::Vec2& offset,
                                  const cocos2d::Size& originalSize,
                                  const cocos2d::Rect& capInsets) override;

private:
    cocos2d::GLProgramState* createState();

    cocos2d::GLProgramState* customState_;
};
NS_EE_END

#endif /* EE_LIBRARY_SCALE9SPRITE_WITH_HSV_HPP_ */
