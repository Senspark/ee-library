//
//  DialogTestScene.hpp
//  ee-library-test
//
//  Created by Zinge on 4/18/17.
//
//

#ifndef EE_LIBRARY_TEST_DIALOG_TEST_SCENE_HPP_
#define EE_LIBRARY_TEST_DIALOG_TEST_SCENE_HPP_

#include <cocosbuilder/CocosBuilder.h>
#include <ui/UIWidget.h>
#include <EEForward.hpp>
#include <EECocos2dxFwd.hpp>

class DialogTestScene : public cocos2d::ui::Widget,
                        public cocosbuilder::NodeLoaderListener,
                        public cocosbuilder::CCBMemberVariableAssigner,
                        public cocosbuilder::CCBSelectorResolver {
private:
    using Super = cocos2d::ui::Widget;

public:
    using Loader = ee::GenericLoader<DialogTestScene, ee::UiWidgetLoader>;

    static void openScene();

protected:
    friend Loader;

    CREATE_FUNC(DialogTestScene);

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void onNodeLoaded(cocos2d::Node* node,
                              cocosbuilder::NodeLoader* nodeLoader) override;

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget,
                                           const char* pMemberVariableName,
                                           cocos2d::Node* pNode) override;

    virtual cocos2d::SEL_MenuHandler
    onResolveCCBCCMenuItemSelector(cocos2d::Ref* pTarget,
                                   const char* pSelectorName) override;

    virtual cocos2d::extension::Control::Handler
    onResolveCCBCCControlSelector(cocos2d::Ref* pTarget,
                                  const char* pSelectorName) override;

    virtual void resume() override;
    virtual void pause() override;

private:
    enum class AnimationMode {
        Instant,
        Delayed,
    };

    AnimationMode pushMode_;
    AnimationMode popMode_;

    void onPushModeButtonPressed();
    void onPopModeButtonPressed();
    void onPushButtonPressed();
    void onPopButtonPressed();
    void onPushAndPopButtonPressed();
    void onPopAndPushButtonPressed();
    void onChangeButtonPressed();

    void push();
    void pop();
    void change();

    cocos2d::ui::Button* pushModeButton_;
    cocos2d::ui::Button* popModeButton_;
    cocos2d::ui::Button* pushButton_;
    cocos2d::ui::Button* popButton_;
    cocos2d::ui::Button* pushAndPopButton_;
    cocos2d::ui::Button* popAndPushButton_;
    cocos2d::ui::Button* changeButton_;
};

#endif /* EE_LIBRARY_TEST_DIALOG_TEST_SCENE_HPP_ */
