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

#include <GUI/CCControlExtension/CCControl.h>
#include <ui/UIWidget.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
struct DummyRef : public cocos2d::Ref {
    void dummyFunction(cocos2d::Ref*) {}
    void dummyFunction(cocos2d::Ref*, cocos2d::extension::Control::EventType) {}
};

/// Converts touch callback
/// from @c cocos2d::extension::ControlButton
/// to @c cocos2d::ui::Button.
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

#define CCB_SELECTORRESOLVER_CCBUTTON_TOUCH(target, selectorName, callback) \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) { \
        ee::detail::UiWidgetCallback::getInstance()->setActiveTouchCallback(callback); \
        return CC_MENU_SELECTOR(ee::detail::DummyRef::dummyFunction); \
    }

#define CCB_SELECTORRESOLVER_CCBUTTON_TOUCH_EX(selectorName, callback) \
    CCB_SEELCTORRESOLVER_CCBUTTON_TOUCH(this, selectorName, callback)

#define CCB_SELECTORRESOLVER_CCBUTTON_CLICK(target, selectorName, callback) \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) { \
        ee::detail::UiWidgetCallback::getInstance()->setActiveClickCallback(callback); \
        return CC_MENU_SELECTOR(ee::detail::DummyRef::dummyFunction); \
    }
NS_DETAIL_END
NS_EE_END

#endif /* EEUiWidgetCallback_hpp */
