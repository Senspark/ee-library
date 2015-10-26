//
//  CCBUtils.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 5/26/15.
//
//

#include "cocos2d.h"
#include "cocosbuilder/cocosbuilder.h"

#include "CCBUtils.hpp"

#include "TestScene.hpp"

#include "TemplateLoader.h"

void CCBUtils::initLoaders() {
    
    auto library = cocosbuilder::NodeLoaderLibrary::getInstance();
    
    library->registerNodeLoader("TestScene", TemplateLoader<TestScene, cocosbuilder::NodeLoader>::loader());
}

cocos2d::Scene* CCBUtils::openScene(const std::string& filename) {
    auto reader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
    auto scene = reader->createSceneWithNodeGraphFromFile(filename.c_str());
    reader->release();
    auto transition = cocos2d::TransitionFade::create(0.5f, scene, cocos2d::Color3B(0, 0, 0));
    cocos2d::Director::getInstance()->replaceScene(transition);
    return scene;
}

cocos2d::Node* CCBUtils::openNode(const std::string& filename) {
    auto reader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
    auto node = reader->readNodeGraphFromFile(filename.c_str());
    reader->release();
    return node;
}
