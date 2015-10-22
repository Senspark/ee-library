#ifndef EE_LIBRARY_TEST_SCENE_HPP
#define EE_LIBRARY_TEST_SCENE_HPP

#include "2d/CCNode.h"

class TestScene : public cocos2d::Node {
public:
    static TestScene* create();
    static cocos2d::Scene* openScene();
    
private:
    class Impl;
};

#endif // EE_LIBRARY_TEST_SCENE_HPP
