//
//  EEGroupNode.h
//  roll-eat
//
//  Created by enrevol on 9/29/15.
//
//

#ifndef EE_LIBRARY_GROUP_NODE_H
#define EE_LIBRARY_GROUP_NODE_H

#include "EEForward.hpp"
#include "EEMacro.hpp"

#include <functional>
#include <memory>

namespace_ee_begin
class GroupNode {
protected:
    using EaseAction = std::function<cocos2d::ActionEase*(cocos2d::ActionFloat*)>;
    
    GroupNode();
    virtual ~GroupNode();
    
    void setDuration(float duration);
    void setMinPositionX(float positionX);
    
    void setBackground(cocos2d::ui::Scale9Sprite* background, float minWidth, float maxWidth);
    void addButton(cocos2d::Node* button, float maxPositionX);
    
    void setMinimizeEaseAction(const EaseAction& callback);    
    void setMaximizeEaseAction(const EaseAction& callback);
    
    void maximize();
    void minimize();
    
    void instantlyMaximize();
    void instantlyMinimize();
    
private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
namespace_ee_end

#endif /* EE_LIBRARY_GROUP_NODE_H */
