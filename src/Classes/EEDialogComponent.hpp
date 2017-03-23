//
//  EEDialogComponent.hpp
//  ee-library
//
//  Created by Zinge on 3/23/17.
//
//

#ifndef EE_LIBRARY_DIALOG_COMPONENT_HPP_
#define EE_LIBRARY_DIALOG_COMPONENT_HPP_

#include <2d/CCComponent.h>

namespace ee {
namespace dialog {
class DialogComponent : public cocos2d::Component {
private:
    using Super = cocos2d::Component;

public:
    using ResumeCallback = std::function<void()>;
    using PauseCallback = std::function<void()>;

    static const std::string DefaultName;

    static DialogComponent* create();

    void resume();
    void pause();

    DialogComponent* setResumeCallback(const ResumeCallback& callback);
    DialogComponent* setPauseCallback(const PauseCallback& callback);

private:
    virtual bool init() override;

    ResumeCallback resumeCallback_;
    PauseCallback pauseCallback_;
};
} // namespace dialog
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_COMPONENT_HPP_ */
