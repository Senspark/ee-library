//
//  EEUiWidgetCallback.cpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#include "EEUiWidgetCallback.hpp"

NS_EE_BEGIN
UiWidgetCallback* UiWidgetCallback::getInstance() {
    static UiWidgetCallback sharedInstance;
    return &sharedInstance;
}

void UiWidgetCallback::setActiveClickCallback(const ClickCallback& callback) {
    _clickCallback = callback;
    _touchCallback = nullptr;
}

void UiWidgetCallback::setActiveTouchCallback(const TouchCallback& callback) {
    _touchCallback = callback;
    _clickCallback = nullptr;
}

auto UiWidgetCallback::getActiveTouchCallback() const -> const TouchCallback& {
    return _touchCallback;
}

auto UiWidgetCallback::getActiveClickCallback() const -> const ClickCallback& {
    return _clickCallback;
}
NS_EE_END