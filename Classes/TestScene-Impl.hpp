//
//  TestScene-Impl.hpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#ifndef EE_LIBRARY_TEST_SCENE_IMPL_HPP
#define EE_LIBRARY_TEST_SCENE_IMPL_HPP

#include "TestScene.hpp"
#include "cocos2d.h"
#include "CocosGUI.h"
#include "cocosbuilder/cocosbuilder.h"
#include "cocos-ext.h"

class TestScene::Impl : public TestScene, public cocosbuilder::CCBMemberVariableAssigner, public cocosbuilder::CCBSelectorResolver {
public:
    virtual bool init() override;
    virtual void onEnter() override;
    
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) override;
    virtual bool onAssignCCBCustomProperty(cocos2d::Ref* pTarget, const char* pMemberVariableName, const cocos2d::Value& value) override { return false; };
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override { return nullptr; }
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override { return nullptr; }
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override;
    
    void pressedReset(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressedProcess(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    
    void pressed1pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed2pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed4pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed8pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed16pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed32pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed64pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed128pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressed256pxButton(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    
    void pressedGaussianBlur(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressedBoxBlur1D(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    void pressedBoxBlur2D(cocos2d::Ref*, cocos2d::extension::Control::EventType);
    
    int _range;
    
    enum class Filter {
        None,
        GaussianBlur,
        BoxBlur1D,
        BoxBlur2D
    } _filter;
    
    void updateRange(int range);
    void updateFilter(Filter filter);
    
    cocos2d::Sprite* _imageSprite;
    cocos2d::Label*  _rangeLabel;
    cocos2d::Label*  _filterLabel;
    cocos2d::Label*  _timeLabel;
};

#endif /* TestScene_Impl_h */
