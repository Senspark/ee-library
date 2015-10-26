//
//  EEGroupButtonDelegate.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/29/15.
//
//

#ifndef EE_LIBRARY_GROUP_NODE_DELEGATE_H
#define EE_LIBRARY_GROUP_NODE_DELEGATE_H

#include "EEMacro.hpp"

namespace_ee_begin
class GroupNodeDelegate {
protected:
    friend class GroupNode;
    
    virtual void onMinimizationBegan() {}
    virtual void onMinimizationEnded() {}
    virtual void onMaximizationBegan() {}
    virtual void onMaximizationEnded() {}
};
namespace_ee_end

#endif /* EE_LIBRARY_GROUP_NODE_DELEGATE_H */