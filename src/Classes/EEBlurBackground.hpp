//
//  EEBlurBackground.hpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

#ifndef EE_LIBRARY_BLUR_BACKGROUND_HPP_
#define EE_LIBRARY_BLUR_BACKGROUND_HPP_

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <2d/CCNode.h>

NS_EE_BEGIN
class BlurBackground : public cocos2d::Node {
public:
    CREATE_FUNC(BlurBackground);
    
    void setRenderScale(float scale);
    
protected:
    virtual bool init() override;
    
    virtual void visit(cocos2d::Renderer* renderer,
                       const cocos2d::Mat4& parentTransforms,
                       std::uint32_t parentFlags) override;
    
    void initRenderers();    
    void resetRenderers();
    
    float renderScale_;
    
    cocos2d::RenderTexture* verticalRenderer_;
    cocos2d::RenderTexture* horizontalRenderer_;
};
NS_EE_END

#endif /* EE_LIBRARY_BLUR_BACKGROUND_HPP_ */
