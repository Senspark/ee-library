//
//  EETransitionDialog.hpp
//  ee-library
//
//  Created by enrevol on 12/1/15.
//
//

#ifndef EE_LIBRARY_TRANSITION_SCENE_HPP_
#define EE_LIBRARY_TRANSITION_SCENE_HPP_

#include "EEMacro.hpp"
#include "EEForward.hpp"
#include "cocos2d.h"

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace_ee_begin
class TransitionScene : public cocos2d::TransitionScene {
public:
    class Builder;
    
    /**
     * Runs the transition scene.
     */
    void run();
    
protected:
    virtual ~TransitionScene();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    /**
     * Starts asynchronously loading textures.
     * Change phase from Phase::Pre to Phase::In.
     */
    void loadTexture();
    
    /**
     * Called when a texture has been loaded.
     */
    virtual void onTextureLoaded(const std::string& filename, cocos2d::Texture2D* texure) {}
    
    /**
     * Called after loadTexture is called.
     */
    virtual void onLoadingTextureBegan() {}
    
    /**
     * Called after all textures has been loaded.
     */
    virtual void onLoadingTextureEnded() {}
    
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, std::uint32_t flags) override;
    
    const std::queue<std::string>& getFilenames() const;
    
    enum class Phase { Pre, In, Post };
    
    Phase getCurrentPhase() const;
    
    virtual void onPhaseChanged(Phase phase) {}
    
private:
    void loadNextTexture();
    void onTextureLoadedCallback(const std::string& filename, cocos2d::Texture2D* texture);
    void onLoadingTextureEndedCallback();
    
    std::function<cocos2d::Scene*()> _inSceneCallback;
    std::queue<std::string> _filenames;
    
    Phase _currentPhase;
};

class TransitionScene::Builder {
public:
    virtual ~Builder();
    
    virtual Builder& setInSceneCallback(const std::function<cocos2d::Scene*()>& callback);
    virtual Builder& addImageFilename(const std::string& filename);
    virtual Builder& addImageFilenames(const std::vector<std::string>& filenames);
    
    virtual TransitionScene* build() const;
    
protected:
    virtual TransitionScene* create() const;
    
private:
    std::function<cocos2d::Scene*()> _inSceneCallback;
    std::queue<std::string> _filenames;
};
namespace_ee_end

#endif /* EE_LIBRARY_TRANSITION_SCENE_HPP_ */
