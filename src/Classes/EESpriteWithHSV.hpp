//
//  EESpriteWithHSV.hpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#ifndef EE_LIBRARY_SPRITE_WITH_HSV_HPP_
#define EE_LIBRARY_SPRITE_WITH_HSV_HPP_

#include "EEMacro.hpp"
#include "EEForward.hpp"

#include <2d/CCSprite.h>
#include <math/Mat4.h>

NS_EE_BEGIN
class SpriteWithHSV : public cocos2d::Sprite {
private:
    using Initializer = std::function<bool(SpriteWithHSV* instance)>;

    static SpriteWithHSV* createImpl(const Initializer& initializer);

public:
    using Loader = SpriteWithHSVLoader;

    static SpriteWithHSV* create();

    static SpriteWithHSV* create(const std::string& filename);

    static SpriteWithHSV* create(const std::string& filename,
                                 const cocos2d::Rect& rect);

    static SpriteWithHSV* createWithTexture(cocos2d::Texture2D* texture);

    static SpriteWithHSV* createWithTexture(cocos2d::Texture2D* texture,
                                            const cocos2d::Rect& rect,
                                            bool rotated = false);

    static SpriteWithHSV*
    createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

    static SpriteWithHSV*
    createWithSpriteFrameName(const std::string& spriteFrameName);

    /// Assigns the hue rotation (in degrees).
    ///
    /// Default is 0.0.
    void setHue(float hue);

    /// Default is 1.0.
    void setSaturation(float saturation);

    /// Default is 0.0.
    void setBrightness(float brightness);

    /// Default is 1.0.
    void setContrast(float contrast);

protected:
    virtual bool initWithTexture(cocos2d::Texture2D* texture) override;
    
    virtual bool initWithTexture(cocos2d::Texture2D* texture,
                                 const cocos2d::Rect& rect,
                                 bool rotated) override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

    void initShader();

    void updateMatrix();
    void updateHueMatrix();
    void updateSaturationMatrix();
    void updateBrightnessMatrix();
    void updateContrastMatrix();

    void uniformCallback(cocos2d::GLProgram* p, cocos2d::Uniform* u);

    float hue_;
    float saturation_;
    float brightness_;
    float contrast_;

    cocos2d::Mat4 hsvMatrix_;
    cocos2d::Mat4 hueMatrix_;
    cocos2d::Mat4 saturationMatrix_;
    cocos2d::Mat4 brightnessMatrix_;
    cocos2d::Mat4 contrastMatrix_;

    bool matrixDirty_;
    bool hueMatrixDirty_;
    bool saturationMatrixDirty_;
    bool brightnessMatrixDirty_;
    bool contrastMatrixDirty_;
};
NS_EE_END

#endif /* EE_LIBRARY_SPRITE_WITH_HSV_HPP_ */
