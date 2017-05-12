//
//  EEAction.cpp
//  SDE-PRO
//
//  Created by Zinge on 3/24/17.
//
//

#include "EEAction.hpp"
#include "EEUtils.hpp"

#include <2d/CCActionInstant.h>
#include <base/CCRefPtr.h>

namespace ee {
namespace {
class ExtraAction : public cocos2d::FiniteTimeAction {
public:
    static ExtraAction* create();
    virtual ExtraAction* clone() const override;
    virtual ExtraAction* reverse(void) const override;
    virtual void update(float time) override;
    virtual void step(float delta) override;
};

ExtraAction* ExtraAction::create() {
    auto result = new ExtraAction();
    result->autorelease();
    return result;
}
ExtraAction* ExtraAction::clone() const {
    return create();
}

ExtraAction* ExtraAction::reverse() const {
    return create();
}

void ExtraAction::update(float time) {
    CC_UNUSED_PARAM(time);
}

void ExtraAction::step(float delta) {
    CC_UNUSED_PARAM(delta);
}
} // namespace

Sequence* Sequence::create() {
    auto result = new Sequence();
    result->autorelease();
    result->initWithTwoActions(ExtraAction::create(), ExtraAction::create());
    return result;
}

Sequence* Sequence::then(cocos2d::FiniteTimeAction* action) {
    auto guard0 = ee::make_ref_guard(_actions[0]);
    auto guard1 = ee::make_ref_guard(_actions[1]);
    _actions[0]->release();
    _actions[1]->release();
    if (count_ == 0) {
        initWithTwoActions(action, _actions[1]);
    } else if (count_ == 1) {
        initWithTwoActions(_actions[0], action);
    } else {
        initWithTwoActions(
            Sequence::create()->then(_actions[0])->then(_actions[1]), action);
    }
    ++count_;
    return this;
}

Sequence* Sequence::delay(float duration) {
    return then(cocos2d::DelayTime::create(duration));
}

Sequence* Sequence::then(const std::function<void()>& callback) {
    return then(cocos2d::CallFunc::create(callback));
}
} // namespace ee
