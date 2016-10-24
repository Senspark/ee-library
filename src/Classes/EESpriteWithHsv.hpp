//
//  EESpriteWithHSV.hpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#ifndef EE_LIBRARY_SPRITE_WITH_HSV_HPP_
#define EE_LIBRARY_SPRITE_WITH_HSV_HPP_

#include "EEHsvProtocol.hpp"

#include <2d/CCSprite.h>
#include <math/Mat4.h>

namespace ee {
class SpriteWithHsvLoader;

class SpriteWithHsv : public cocos2d::Sprite, public HsvProtocol {
private:
    using Initializer = std::function<bool(SpriteWithHsv* instance)>;

    static SpriteWithHsv* createImpl(const Initializer& initializer);

public:
    using Loader = SpriteWithHsvLoader;

    static SpriteWithHsv* create();

    static SpriteWithHsv* create(const std::string& filename);

    static SpriteWithHsv* create(const std::string& filename,
                                 const cocos2d::Rect& rect);

    static SpriteWithHsv* createWithTexture(cocos2d::Texture2D* texture);

    static SpriteWithHsv* createWithTexture(cocos2d::Texture2D* texture,
                                            const cocos2d::Rect& rect,
                                            bool rotated = false);

    static SpriteWithHsv*
    createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

    static SpriteWithHsv*
    createWithSpriteFrameName(const std::string& spriteFrameName);

protected:
    using Sprite::initWithTexture;

    virtual bool initWithTexture(cocos2d::Texture2D* texture,
                                 const cocos2d::Rect& rect,
                                 bool rotated) override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

    void initShader();
};
} // namespace ee

#endif /* EE_LIBRARY_SPRITE_WITH_HSV_HPP_ */
