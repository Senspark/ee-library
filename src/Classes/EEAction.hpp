//
//  EEAction.hpp
//  SDE-PRO
//
//  Created by Zinge on 3/24/17.
//
//

#ifndef EE_LIBRARY_ACTION_HPP_
#define EE_LIBRARY_ACTION_HPP_

#include <2d/CCActionInterval.h>

namespace ee {
class Sequence : public cocos2d::Sequence {
private:
    using Super = cocos2d::Sequence;

public:
    static Sequence* create();

    Sequence* delay(float duration);

    Sequence* then(const std::function<void()>& callback);

    Sequence* then(cocos2d::FiniteTimeAction* action);

private:
    std::size_t count_;
};
} // namespace ee

#endif /* EE_LIBRARY_ACTION_HPP_ */
