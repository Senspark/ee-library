//
//  EEActiveControl.cpp
//  ee-library
//
//  Created by enrevol on 10/29/15.
//
//

#include "EEActiveControl.hpp"

namespace_ee_begin
ActiveNodeButton* ActiveNodeButton::create() {
    auto ret = new (std::nothrow) ActiveNodeButton();
    if (ret != nullptr && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ActiveNodeButton::onEnter() {
    NodeButton::onEnter();
    setEnabled(true);
}

void ActiveNodeButton::onExit() {
    NodeButton::onExit();
    setEnabled(false);
}

ActiveButton* ActiveButton::create() {
    auto ret = new (std::nothrow) ActiveButton();
    if (ret != nullptr && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ActiveButton::onEnter() {
    Button::onEnter();
    setEnabled(true);
}

void ActiveButton::onExit() {
    Button::onExit();
    setEnabled(false);
}
namespace_ee_end