//
//  EESceneSwitcher.hpp
//  ee-library
//
//  Created by Zinge on 4/21/17.
//
//

#ifndef EE_LIBRARY_SCENE_SWITCHER_HPP_
#define EE_LIBRARY_SCENE_SWITCHER_HPP_

#include "EECocos2dxFwd.hpp"

#include <2d/CCTransition.h>

namespace ee {
/// Layout:
/// - out-scene on exit transition did start.
/// - pre-phase actions.
/// - out-scene on exit.
/// - in-phase actions.
/// - in-scene constructor.
/// - in-scene on enter.
/// - pose-phase actions.
/// - in-scene on enter transition did finish.
class SceneSwitcher : public cocos2d::TransitionScene {
private:
    using Super = cocos2d::Scene;

public:
    using SceneConstructor = std::function<cocos2d::Scene*()>;

    /// Sets the in-scene constructor.
    /// Used to constructor the in-scene.
    /// @param constructor The in-scene constructor.
    SceneSwitcher* setInSceneConstructor(const SceneConstructor& constructor);

    /// Adds a pre-phase action.
    SceneSwitcher* addPrePhaseAction(cocos2d::FiniteTimeAction* action);

    /// Adds an in-phase action.
    SceneSwitcher* addInPhaseAction(cocos2d::FiniteTimeAction* action);

    /// Adds a pose-phase action.
    SceneSwitcher* addPosePhaseAction(cocos2d::FiniteTimeAction* action);

    void run();

protected:
    enum class Phase { None, Pre, In, Post };

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;

    virtual void onExitTransitionDidStart() override;
    virtual void onExit() override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

private:
    void onPhaseBegan(Phase phase);
    void onPhaseEnded(Phase phase);

    Phase phase_;
    SceneConstructor inSceneConstructor_;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> preActions_;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> inActions_;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> postActions_;

    cocos2d::Node* actor_;
};
} // namespace ee

#endif /* EE_LIBRARY_SCENE_SWITCHER_HPP_ */
