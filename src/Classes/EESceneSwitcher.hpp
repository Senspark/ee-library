//
//  EESceneSwitcher.hpp
//  ee-library
//
//  Created by Zinge on 4/21/17.
//
//

#ifndef EE_LIBRARY_SCENE_SWITCHER_HPP_
#define EE_LIBRARY_SCENE_SWITCHER_HPP_

#include <queue>

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
    using LayerConstructor = std::function<cocos2d::Node*()>;

    CREATE_FUNC(SceneSwitcher);

    /// Sets the in-scene constructor.
    /// Used to constructor the in-scene.
    /// @param constructor The in-scene constructor.
    SceneSwitcher* setInSceneConstructor(const SceneConstructor& constructor);

    /// Sets the layer constructor.
    /// Used to constructor a layer in the in-scene.
    /// @param constructor The layer constructor.
    SceneSwitcher* setInLayerConstructor(const LayerConstructor& constructor);

    /// Adds an image for loading in in-phase.
    /// @param imageName The name of the image.
    SceneSwitcher* addImage(const std::string& imageName);

    /// Adds an atlas for loading in in-phase.
    /// @param plistName The name of the plist file.
    /// @param imageName The name of the corresponding image.
    SceneSwitcher* addAtlas(const std::string& plistName,
                            const std::string& imageName);

    /// Adds a pre-phase action.
    SceneSwitcher* addPrePhaseAction(cocos2d::FiniteTimeAction* action);

    /// Adds an in-phase action.
    SceneSwitcher* addInPhaseAction(cocos2d::FiniteTimeAction* action);

    /// Adds a post-phase action.
    SceneSwitcher* addPostPhaseAction(cocos2d::FiniteTimeAction* action);

    void run();

protected:
    enum class Phase { None, Pre, In, Post };

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;

    virtual void onExitTransitionDidStart() override;
    virtual void onExit() override;

    virtual void visit(cocos2d::Renderer* renderer,
                       const cocos2d::Mat4& transform,
                       std::uint32_t flags) override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

private:
    cocos2d::Scene* createInScene() const;

    void onPhaseBegan(Phase phase);
    void onPhaseEnded(Phase phase);

    bool loadNextItem();
    bool loadNextImage();
    bool loadNextAtlas();

    void onImageLoaded(cocos2d::Texture2D* texture,
                       const std::string& imageName);

    void onAtlasLoaded(cocos2d::Texture2D* texture,
                       const std::string& plistName,
                       const std::string& imageName);

    Phase phase_;
    bool imagesLoaded_;
    bool inActionsDone_;

    SceneConstructor inSceneConstructor_;
    LayerConstructor inLayerConstructor_;

    std::size_t loadedImageCount_;
    std::vector<std::string> images_;

    std::size_t loadedAtlasCount_;
    std::vector<std::pair<std::string, std::string>> atlases_;

    cocos2d::Vector<cocos2d::FiniteTimeAction*> preActions_;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> inActions_;
    cocos2d::Vector<cocos2d::FiniteTimeAction*> postActions_;

    cocos2d::Node* actor_;
};
} // namespace ee

#endif /* EE_LIBRARY_SCENE_SWITCHER_HPP_ */
