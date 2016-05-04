//
//  EEAsyncSpriteLoader.hpp
//  ee-library
//
//  Created by enrevol on 4/14/16.
//
//

#ifndef EEAsyncSpriteLoader_hpp
#define EEAsyncSpriteLoader_hpp

#include "EEMacro.hpp"

#include <cocosbuilder/CCSpriteLoader.h>

NS_EE_BEGIN
class AsyncSpriteLoader : public cocosbuilder::SpriteLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AsyncSpriteLoader, loader);
    
private:
    virtual cocos2d::Sprite* createNode(cocos2d::Node* parent,
                                        cocosbuilder::CCBReader* reader)
};
NS_EE_END

#endif /* EEAsyncSpriteLoader_hpp */
