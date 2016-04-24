//
//  EEUtils.cpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#include "EEUtils.hpp"

#include <cocos2d.h>

NS_EE_BEGIN
void doRecursively(cocos2d::Node* node,
                   const std::function<void(cocos2d::Node*)>& action) {
    action(node);
    auto&& children = node->getChildren();
    for (auto&& child : children) {
        doRecursively(child, action);
    }
}

void pauseAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::pause);
}

void resumeAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::resume);
}

bool isActuallyVisible(const cocos2d::Node* node) {
    if (node == nullptr) {
        // node is nullptr.
        return false;
    }
    if (node->isRunning() == false) {
        // node is not running.
        return false;
    }
    auto current = node;
    while (current != nullptr) {
        if (current->isVisible() == false) {
            // One of node's ancestors is not visible.
            return false;
        }
        current = current->getParent();
    }
    return true;
}
NS_EE_END
