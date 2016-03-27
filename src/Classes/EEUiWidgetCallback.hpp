//
//  EEUiWidgetCallback.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EEUiWidgetCallback_hpp
#define EEUiWidgetCallback_hpp

#include "EEMacro.hpp"

#include "GUI/CCControlExtension/CCControl.h"
#include "ui/UIWidget.h"

NS_EE_BEGIN
NS_DETAIL_BEGIN
struct DummyRef : public cocos2d::Ref {
    void dummyFunction(cocos2d::Ref*, cocos2d::extension::Control::EventType) {}
};
NS_DETAIL_END
class UiWidgetCallback {
public:
    using TouchCallback = cocos2d::ui::Widget::ccWidgetTouchCallback;
    using ClickCallback = cocos2d::ui::Widget::ccWidgetClickCallback;
    
    static UiWidgetCallback* getInstance();
    
    void setActiveTouchCallback(const TouchCallback& callback);
    void setActiveClickCallback(const ClickCallback& callback);
    
    const TouchCallback& getActiveTouchCallback() const;
    const ClickCallback& getActiveClickCallback() const;
    
private:
    TouchCallback _touchCallback;
    ClickCallback _clickCallback;
};

#define CCB_SELECTORRESOLVER_CCCONTROL_TOUCH(target, selectorName, callable) \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) { \
        ::ee::UiWidgetCallback::getInstance()->setActiveTouchCallback(callable); \
        return static_cast<cocos2d::extension::Control::Handler>(&::ee::detail::DummyRef::dummyFunction); \
    }

#define CCB_SELECTORRESOLVER_CCCONTROL_TOUCH_EX(selectorName, callable) \
    CCB_SELECTORRESOLVER_CCCONTROL_TOUCH(this, selectorName, callable)

#define CCB_SELECTORRESOLVER_CCCONTROL_CLICK(target, selectorName, callable) \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) { \
        ::ee::UiWidgetCallback::getInstance()->setActiveClickCallback(callable); \
        return static_cast<cocos2d::extension::Control::Handler>(&::ee::detail::DummyRef::dummyFunction); \
    }

#define CCB_SELECTORRESOLVER_CCCONTROL_CLICK_EX_0(function) { \
    std::string str = # function; \
    str = str.substr(str.rfind("::") + 2); \
    CCB_SELECTORRESOLVER_CCCONTROL_CLICK(this, str.c_str(), CC_CALLBACK_0(function, this)); \
}

#define CCB_SELECTORRESOLVER_CCCONTROL_CLICK_EX_1(function) { \
    std::string str = # function; \
    str = str.substr(str.rfind("::") + 2); \
    CCB_SELECTORRESOLVER_CCCONTROL_CLICK(this, str.c_str(), CC_CALLBACK_1(function, this)); \
}
NS_EE_END

#endif /* EEUiWidgetCallback_hpp */
