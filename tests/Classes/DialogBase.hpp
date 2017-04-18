//
//  DialogBase.hpp
//  ee-library-test
//
//  Created by Zinge on 4/18/17.
//
//

#ifndef EE_LIBRARY_TEST_DIALOG_BASE_HPP_
#define EE_LIBRARY_TEST_DIALOG_BASE_HPP_

#include <cocosbuilder/CocosBuilder.h>
#include <EEForward.hpp>
#include <EECocos2dxFwd.hpp>
#include <EEDialog.hpp>

class DialogBase : public ee::Dialog,
                   public cocosbuilder::NodeLoaderListener,
                   public cocosbuilder::CCBMemberVariableAssigner {
private:
    using Super = ee::Dialog;

public:
    using Loader = ee::GenericLoader<DialogBase, ee::UiWidgetLoader>;

    static DialogBase* openNode();

    DialogBase* addShowAnimation(float duration);
    DialogBase* addHideAnimation(float duration);

    virtual void setContentSize(const cocos2d::Size& size) override;

protected:
    friend Loader;

    CREATE_FUNC(DialogBase);

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void onDialogWillShow() override;
    virtual void onDialogWillHide() override;

    virtual void onNodeLoaded(cocos2d::Node* node,
                              cocosbuilder::NodeLoader* nodeLoader) override;

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget,
                                           const char* pMemberVariableName,
                                           cocos2d::Node* pNode) override;

private:
    bool animateShow_;
    bool animateHide_;

    float showingDuration_;
    float hidingDuration_;

    cocos2d::Node* background_;
};

#endif /* EE_LIBRARY_TEST_DIALOG_BASE_HPP_ */
