//
//  EEGroupNode-Impl.h
//  roll-eat
//
//  Created by enrevol on 9/29/15.
//
//

#ifndef EE_LIBRARY_GROUP_NODE_IMPL_H
#define EE_LIBRARY_GROUP_NODE_IMPL_H

#include "EEGroupNode.hpp"

#include <vector>

namespace_ee_begin
class GroupNode::Impl {
public:
    cocos2d::ActionInterval* createMinimizeAction(cocos2d::ActionFloat* action);
    cocos2d::ActionInterval* createMaximizeAction(cocos2d::ActionFloat* action);
    
    float _duration;
    
    cocos2d::ui::Scale9Sprite* _background;
    float _backgroundMinWidth;
    float _backgroundMaxWidth;
    
    float _minPositionX;
    std::vector<std::pair<cocos2d::Node*, float>> _buttonList;
    
    EaseAction _minimizeEaseAction;
    EaseAction _maximizeEaseAction;
    
    static const int ActionTag;
};
namespace_ee_end

#endif // EE_LIBRARY_GROUP_NODE_IMPL_H