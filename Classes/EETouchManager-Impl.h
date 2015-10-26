//
//  EETouchManager-Impl.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/4/15.
//
//

#ifndef EE_LIBRARY_TOUCH_MANAGER_IMPL_H
#define EE_LIBRARY_TOUCH_MANAGER_IMPL_H

#include "EETouchManager.h"
#include <unordered_set>

namespace_ee_begin
class TouchManager::Impl : public TouchManager {
public:
    cocos2d::Touch* _activeTouch;
};
namespace_ee_end

#endif // EE_LIBRARY_TOUCH_MANAGER_IMPL_H