//
//  EEDialog.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#ifndef EE_LIBRARY_DIALOG_HPP_
#define EE_LIBRARY_DIALOG_HPP_

#include <functional>
#include <memory>
#include <vector>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <base/CCRefPtr.h>
#include <ui/UIWidget.h>

NS_EE_BEGIN
namespace dialog {
class Dialog;
} // namespace dialog

using dialog::Dialog;

namespace dialog {
/// Dialog lifetime:
/// Initial state:
/// - Inactive (isActive() returns false).
///
/// When show() is called.
/// - onDialogWillShow is called.
///   - invoke all callbacks in addDialogWillShowCallback() (can be overroden).
/// - onDialogDidShow is called.
///   - invoke all callbacks in addDialogDidShowCallback() (can be overroden).
/// - Active (isActive() returns true).
///
/// When hide() is called.
/// - Inactive (isActive() returns false).
/// - onDialogWillHide is called.
///   - invoke all callbacks in addDialogWillHideCallback() (can be overroden).
/// - onDialogDidHidee is called.
///   - invoke all callbacks in addDialogDidHideCallback() (can be overroden).
class Dialog : public cocos2d::ui::Widget {
private:
    using Super = cocos2d::ui::Widget;

public:
    using CallbackType = std::function<void(Dialog*)>;

    using TransitionType = cocos2d::FiniteTimeAction;
    using TransitionRef = cocos2d::RefPtr<TransitionType>;

    template <class... Args>
    using enable_if_args_are_transition_types = std::enable_if_t<
        (std::is_base_of<TransitionType,
                         std::remove_pointer_t<std::decay_t<Args>>>::value &&
         ...),
        Dialog*>;

    static const int ContainerLocalZOrder;
    static const std::size_t TopLevel;

    /// Attempts to show this dialog to the current scene.
    /// @param level The dialog level to show, should be in range [1, +inf).
    virtual void show(std::size_t level = TopLevel);

    /// Attempts to hide this dialog from the current scene.
    virtual void hide();

    /// Retrieves the dialog level in the current scene.
    std::size_t getDialogLevel() const noexcept;

    /// Retrieves this dialog's container
    virtual cocos2d::Node* getContainer();

    /// Retrieves this dialog's container (const version).
    virtual const cocos2d::Node* getContainer() const;

    virtual void setActive(bool active);

    /// Whether the user can interact with this dialog.
    bool isActive() const noexcept;

    virtual Dialog* addDialogWillShowCallback(const CallbackType& callback,
                                              int priority = 0);

    virtual Dialog* addDialogDidShowCallback(const CallbackType& callback,
                                             int priority = 0);

    virtual Dialog* addDialogWillHideCallback(const CallbackType& callback,
                                              int priority = 0);

    virtual Dialog* addDialogDidHideCallback(const CallbackType& callback,
                                             int priority = 0);

    Dialog*
    setShowingTransitions(const std::vector<TransitionRef>& transitions);
    Dialog* setHidingTransitions(const std::vector<TransitionRef>& transitions);

    Dialog*
    addShowingTransitions(const std::vector<TransitionRef>& transitions);
    Dialog* addHidingTransitions(const std::vector<TransitionRef>& transitions);

    const std::vector<TransitionRef>& getShowingTransitions() const;
    const std::vector<TransitionRef>& getHidingTransitions() const;

    template <class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    setShowingTransitions(Transitions&&... transitions) {
        return setShowingTransitions(
            {std::forward<Transitions>(transitions)...});
    }

    template <class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    setHidingTransitions(Transitions&&... transitions) {
        return setHidingTransitions(
            {std::forward<Transitions>(transitions)...});
    }

    template <class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    addShowingTransitions(Transitions&&... transitions) {
        return addShowingTransitions(
            {std::forward<Transitions>(transitions)...});
    }

    template <class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    addHidingTransitions(Transitions&&... transitions) {
        return addHidingTransitions(
            {std::forward<Transitions>(transitions)...});
    }

protected:
    friend class DialogManager;

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    /// Invoked when the user clicked the outside boundary of this dialog.
    virtual void onClickedOutside();

    virtual void onDialogWillShow();
    virtual void onDialogDidShow();
    virtual void onDialogWillHide();
    virtual void onDialogDidHide();

private:
    using CallbackInfo = std::pair<CallbackType, int>;

    virtual bool hitTest(const cocos2d::Point& pt,
                         const cocos2d::Camera* camera,
                         cocos2d::Vec3* p) const override;

    void invokeCallbacks(std::vector<CallbackInfo>& callbacks);

    std::size_t dialogLevel_;
    bool isActive_;

    std::vector<TransitionRef> showingTransitions_;
    std::vector<TransitionRef> hidingTransitions_;

    std::vector<CallbackInfo> onDialogWillShowCallbacks_;
    std::vector<CallbackInfo> onDialogDidShowCallbacks_;
    std::vector<CallbackInfo> onDialogWillHideCallbacks_;
    std::vector<CallbackInfo> onDialogDidHideCallbacks_;
};
} // namespace dialog.
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_HPP_ */
