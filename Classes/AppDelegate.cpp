#include "AppDelegate.hpp"
#include "cocos2d.h"
#include "TestScene.hpp"
#include "CCBUtils.hpp"

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

// If you want a different context, just modify the value of glContextAttrs.
// It will takes effect on all platforms.
void AppDelegate::initGLContextAttrs() {
    // Set OpenGL context attributions,now can only set six attributions:
    // Red, green, blue, alpha, depth, stencil.
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    cocos2d::GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, don't modify or remove this function.
static int register_all_packages() {
    return 0; // Flag for packages manager.
}

bool AppDelegate::applicationDidFinishLaunching() {
    // Initialize director.
    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    if (glView == nullptr) {
        glView = cocos2d::GLViewImpl::create("EE Library");
        director->setOpenGLView(glView);
    }
    
    auto file = cocos2d::FileUtils::getInstance();
    file->addSearchPath("Resources-gen");
    // file->addSearchResolutionsOrder("Resources-gen/resources-iphone");
    file->addSearchResolutionsOrder("Resources-gen/resources-ipadhd");
    
    director->setContentScaleFactor(4.0f);

    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glView->setDesignResolutionSize(480, 320, ResolutionPolicy::FIXED_HEIGHT);
    auto&& frameSize = glView->getFrameSize();

    register_all_packages();
    
    CCBUtils::initLoaders();

    // create a scene. it's an autorelease object
    auto scene = TestScene::openScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    cocos2d::Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    cocos2d::Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}