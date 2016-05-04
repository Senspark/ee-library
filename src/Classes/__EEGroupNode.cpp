//
//  EEGroupNode.cpp
//  roll-eat
//
//  Created by enrevol on 9/29/15.
//
//

#include "EEGroupNode-Impl.hpp"
#include "EEGroupNodeDelegate.hpp"
#include "EETouchListener.hpp"

#include "CocosGUI.h"

namespace_ee_begin
const int GroupNode::Impl::ActionTag = 123333;
    
GroupNode::GroupNode() : _impl(new Impl()) {}
GroupNode::~GroupNode() {}
    
void GroupNode::setDuration(float duration) {
    _impl->_duration = duration;
}
    
void GroupNode::setMinPositionX(float positionX) {
    _impl->_minPositionX = positionX;
}

void GroupNode::setBackground(cocos2d::ui::Scale9Sprite* background, float minWidth, float maxWidth) {
    _impl->_background = background;
    _impl->_backgroundMinWidth = minWidth;
    _impl->_backgroundMaxWidth = maxWidth;
}

void GroupNode::addButton(cocos2d::Node* button, float maxPositionX) {
    _impl->_buttonList.emplace_back(button, maxPositionX);
}

void GroupNode::setMinimizeEaseAction(const EaseAction& callback) {
    _impl->_minimizeEaseAction = callback;
}
    
void GroupNode::setMaximizeEaseAction(const EaseAction& callback) {
    _impl->_maximizeEaseAction = callback;
}
    
void GroupNode::maximize() {
    float durationRatio = (_impl->_backgroundMaxWidth - _impl->_background->getContentSize().width) / (_impl->_backgroundMaxWidth - _impl->_backgroundMinWidth);
    float duration = _impl->_duration * durationRatio;
    for (auto&& buttonInfo : _impl->_buttonList) {
        auto button = buttonInfo.first;
        float maxPositionX = buttonInfo.second;
        auto delegate = dynamic_cast<GroupNodeDelegate*>(button);
        button->stopActionByTag(Impl::ActionTag);
        button->setVisible(true);
        auto setter = [button](float positionX) {
            button->setPositionX(positionX);
        };
        auto changePosition = _impl->createMaximizeAction(cocos2d::ActionFloat::create(duration, button->getPositionX(), maxPositionX, setter));
        auto callFunc = cocos2d::CallFunc::create([button, delegate] {
            auto listener = dynamic_cast<TouchListener*>(button);
            if (listener != nullptr) {
                listener->setEnabled(true);
            }
            if (delegate != nullptr) {
                delegate->onMaximizationEnded();
            }
        });
        auto sequence = cocos2d::Sequence::createWithTwoActions(changePosition, callFunc);
        sequence->setTag(Impl::ActionTag);
        button->runAction(sequence);
        if (delegate != nullptr) {
            delegate->onMaximizationBegan();
        }
    }
    _impl->_background->stopActionByTag(Impl::ActionTag);
    auto delegate = dynamic_cast<GroupNodeDelegate*>(this);
    auto setter = [this](float width) {
        _impl->_background->setContentSize(cocos2d::Size(width, _impl->_background->getContentSize().height));
    };
    auto changeWidth = _impl->createMaximizeAction(cocos2d::ActionFloat::create(duration, _impl->_background->getContentSize().width, _impl->_backgroundMaxWidth, setter));
    auto callFunc = cocos2d::CallFunc::create([this, delegate] {
        if (delegate != nullptr) {
            delegate->onMaximizationEnded();
        }
    });
    auto sequence = cocos2d::Sequence::createWithTwoActions(changeWidth, callFunc);
    sequence->setTag(Impl::ActionTag);
    _impl->_background->runAction(sequence);
    if (delegate != nullptr) {
        delegate->onMaximizationBegan();
    }
}

void GroupNode::minimize() {
    float durationRatio = (_impl->_background->getContentSize().width - _impl->_backgroundMinWidth) / (_impl->_backgroundMaxWidth - _impl->_backgroundMinWidth);
    float duration = _impl->_duration * durationRatio;
    for (auto&& buttonInfo : _impl->_buttonList) {
        auto button = buttonInfo.first;
        auto delegate = dynamic_cast<GroupNodeDelegate*>(button);
        auto listener = dynamic_cast<TouchListener*>(button);
        if (listener != nullptr) {
            listener->setEnabled(false);
        }
        button->stopActionByTag(Impl::ActionTag);
        auto setter = [button](float positionX) {
            button->setPositionX(positionX);
        };
        auto changePosition = _impl->createMinimizeAction(cocos2d::ActionFloat::create(duration, button->getPositionX(), _impl->_minPositionX, setter));
        auto callFunc = cocos2d::CallFunc::create([button, delegate] {
            button->setVisible(false);
            if (delegate != nullptr) {
                delegate->onMinimizationEnded();
            }
        });
        auto sequence = cocos2d::Sequence::createWithTwoActions(changePosition, callFunc);
        sequence->setTag(Impl::ActionTag);
        button->runAction(sequence);
        if (delegate != nullptr) {
            delegate->onMinimizationBegan();
        }
    }
    
    _impl->_background->stopActionByTag(Impl::ActionTag);
    auto delegate = dynamic_cast<GroupNodeDelegate*>(this);
    auto setter = [this](float width) {
        _impl->_background->setContentSize(cocos2d::Size(width, _impl->_background->getContentSize().height));
    };
    auto changeWidth = _impl->createMinimizeAction(cocos2d::ActionFloat::create(duration, _impl->_background->getContentSize().width, _impl->_backgroundMinWidth, setter));
    auto callFunc = cocos2d::CallFunc::create([this, delegate] {
        if (delegate != nullptr) {
            delegate->onMinimizationEnded();
        }
    });
    auto sequence = cocos2d::Sequence::createWithTwoActions(changeWidth, callFunc);
    sequence->setTag(Impl::ActionTag);
    _impl->_background->runAction(sequence);
    if (delegate != nullptr) {
        delegate->onMinimizationBegan();
    }
}
    
void GroupNode::instantlyMaximize() {
    for (auto&& buttonInfo : _impl->_buttonList) {
        auto button = buttonInfo.first;
        float maxPositionX = buttonInfo.second;
        button->setPositionX(maxPositionX);
        button->setVisible(true);
        auto listener = dynamic_cast<TouchListener*>(button);
        if (listener != nullptr) {
            listener->setEnabled(true);
        }
    }
    _impl->_background->setContentSize(cocos2d::Size(_impl->_backgroundMaxWidth, _impl->_background->getContentSize().height));
}
    
void GroupNode::instantlyMinimize() {
    for (auto&& buttonInfo : _impl->_buttonList) {
        auto button = buttonInfo.first;
        button->setPositionX(_impl->_minPositionX);
        button->setVisible(false);
        auto listener = dynamic_cast<TouchListener*>(button);
        if (listener != nullptr) {
            listener->setEnabled(false);
        }
    }
    _impl->_background->setContentSize(cocos2d::Size(_impl->_backgroundMinWidth, _impl->_background->getContentSize().height));
}
    
cocos2d::ActionInterval* GroupNode::Impl::createMinimizeAction(cocos2d::ActionFloat* action) {
    if (_minimizeEaseAction != nullptr) {
        return _minimizeEaseAction(action);
    }
    return action;
}

cocos2d::ActionInterval* GroupNode::Impl::createMaximizeAction(cocos2d::ActionFloat* action) {
    if (_maximizeEaseAction != nullptr) {
        return _maximizeEaseAction(action);
    }
    return action;
}
namespace_ee_end